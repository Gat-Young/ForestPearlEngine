#include "FbxToStaticMeshJsonTranslator.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void FbxToStaticMeshJsonTranslator::TranslateDirectory(const std::filesystem::path& FbxInputDirectory, const std::filesystem::path& JsonOutputDirectory)
{
	if (!std::filesystem::exists(FbxInputDirectory))
	{
		std::cerr << "FBX 입력 폴더가 존재하지 않습니다 : " << FbxInputDirectory.string() << "\n";
		return;
	}

	// 출력 폴더들을 미리 생성 (StaticMesh / Material / Texture)
	std::filesystem::create_directories(JsonOutputDirectory / "StaticMesh");
	std::filesystem::create_directories(JsonOutputDirectory / "Material");
	std::filesystem::create_directories(JsonOutputDirectory / "Texture");

	int SuccessCount = 0;
	int FailCount = 0;

	for (const std::filesystem::directory_entry& Entry : std::filesystem::directory_iterator(FbxInputDirectory))
	{
		if (!Entry.is_regular_file())
		{
			continue;
		}

		// 확장자가 .fbx인 파일만 처리 (대소문자 구분 없이)
		std::string Extension = Entry.path().extension().string();
		std::transform(Extension.begin(), Extension.end(), Extension.begin(), [](unsigned char Character) { return static_cast<char>(std::tolower(Character)); });

		if (Extension != ".fbx")
		{
			continue;
		}

		std::cout << "FBX 변환 시작 : " << Entry.path().string() << "\n";

		if (TranslateFbxFile(Entry.path(), JsonOutputDirectory))
		{
			++SuccessCount;
		}
		else
		{
			++FailCount;
		}
	}

	std::cout << "변환 완료 - 성공 : " << SuccessCount << ", 실패 : " << FailCount << "\n";
}

bool FbxToStaticMeshJsonTranslator::TranslateFbxFile(const std::filesystem::path& FbxFilePath, const std::filesystem::path& JsonOutputDirectory)
{
	// FBX 로드 옵션 : 엔진 AssetLoader와 동일하게 좌손 좌표계 / Y-Up / 미터 단위로 맞춘다.
	ufbx_load_opts Opts = {};
	Opts.target_axes = ufbx_axes_left_handed_y_up;
	Opts.target_unit_meters = 1.0f;

	ufbx_error Error;
	ufbx_scene* Scene = ufbx_load_file(FbxFilePath.string().c_str(), &Opts, &Error);

	if (!Scene)
	{
		std::cerr << "FBX 로드 실패 : " << FbxFilePath.string() << " (" << Error.description.data << ")\n";
		return false;
	}

	bool bAnyMeshExported = false;

	// 씬의 모든 노드를 순회하며 메쉬를 가진 노드만 StaticMesh json으로 변환
	for (ufbx_node* Node : Scene->nodes)
	{
		if (!Node || !Node->mesh)
		{
			continue;
		}

		const ufbx_mesh& Mesh = *Node->mesh;

		// 자식 로케이터 노드 -> 소켓 목록
		const json SocketsJson = BuildSocketsJson(*Node);

		// 머티리얼 -> Material.json 생성 및 참조 정보
		const json MaterialJson = ExportMeshMaterial(Mesh, JsonOutputDirectory);

		// 머티리얼에 연결된 텍스처 -> Texture.json 생성 및 참조 배열
		json TextureArrayJson = json::array();
		if (Mesh.materials.count > 0 && Mesh.materials.data[0] != nullptr)
		{
			TextureArrayJson = ExportMaterialTextures(*Mesh.materials.data[0], JsonOutputDirectory);
		}

		const json StaticMeshJson = BuildStaticMeshJson(*Node, SocketsJson, MaterialJson, TextureArrayJson);

		// 파일 이름은 우선 Node 이름 기준, 없으면 메쉬 이름 -> FBX 파일 이름 순으로 대체
		std::string NodeName = ConvertUfbxString(Node->name);
		if (NodeName.empty())
		{
			NodeName = ConvertUfbxString(Mesh.name);
		}
		if (NodeName.empty())
		{
			NodeName = FbxFilePath.stem().string();
		}

		const std::filesystem::path OutputPath = JsonOutputDirectory / "StaticMesh" / (SanitizeFileName(NodeName) + ".json");
		SaveJsonToFile(StaticMeshJson, OutputPath);

		std::cout << "  -> StaticMesh JSON 저장 : " << OutputPath.string() << "\n";

		bAnyMeshExported = true;
	}

	ufbx_free_scene(Scene);

	return bAnyMeshExported;
}

json FbxToStaticMeshJsonTranslator::BuildStaticMeshJson(const ufbx_node& Node, const json& SocketsJson, const json& MaterialJson, const json& TextureArrayJson)
{
	const std::string NodeName = ConvertUfbxString(Node.name);

	json Result;

	Result["Name"] = NodeName;
	Result["NodeName"] = NodeName;

	// 실제 정점/인덱스 버퍼 리소스 추출은 이 툴의 범위가 아니므로 Path는 비워 둔다.
	// (추후 메쉬 리소스 추출 파이프라인이 생기면 이 값을 채우도록 수정)
	json MeshDataEntry;
	MeshDataEntry["Path"] = "";
	MeshDataEntry["Topology"] = "TRIANGLELIST";

	Result["MeshData"] = json::array({ MeshDataEntry });
	Result["Sockets"] = SocketsJson;
	Result["Material"] = MaterialJson;
	Result["Texture"] = TextureArrayJson;

	return Result;
}

json FbxToStaticMeshJsonTranslator::BuildSocketsJson(const ufbx_node& Node)
{
	json SocketsArray = json::array();

	for (ufbx_node* Child : Node.children)
	{
		if (!Child)
		{
			continue;
		}

		// 메쉬를 가진 자식은 별도의 메쉬 노드이므로 소켓에서 제외한다.
		if (Child->mesh)
		{
			continue;
		}

		json SocketJson;
		SocketJson["Name"] = ConvertUfbxString(Child->name);

		json TransformJson;
		TransformJson["Location"] = MakeVec3Json(Child->local_transform.translation);
		TransformJson["Rotation"] = MakeVec3Json(Child->euler_rotation);
		TransformJson["Scale"] = MakeVec3Json(Child->local_transform.scale);

		SocketJson["Transform"] = TransformJson;

		SocketsArray.push_back(SocketJson);
	}

	return SocketsArray;
}

json FbxToStaticMeshJsonTranslator::ExportMeshMaterial(const ufbx_mesh& Mesh, const std::filesystem::path& JsonOutputDirectory)
{
	// 연결된 머티리얼이 없으면 이름이 빈 Material 참조만 반환
	if (Mesh.materials.count == 0 || Mesh.materials.data[0] == nullptr)
	{
		json EmptyMaterial;
		EmptyMaterial["Name"] = "";
		return EmptyMaterial;
	}

	// 우선은 메쉬에 연결된 첫번째 머티리얼만 사용
	const ufbx_material& Material = *Mesh.materials.data[0];
	const std::string MaterialName = ConvertUfbxString(Material.name);

	json MaterialBody;
	MaterialBody["Name"] = MaterialName;
	MaterialBody["BaseColor"] = MakeColorJson(Material.pbr.base_color, 1.0, 1.0, 1.0, 1.0);
	MaterialBody["Diffuse"] = MakeColorJson(Material.fbx.diffuse_color, 1.0, 1.0, 1.0, 1.0);
	MaterialBody["Ambient"] = MakeColorJson(Material.fbx.ambient_color, 0.1, 0.1, 0.1, 1.0);

	json MaterialFileJson;
	MaterialFileJson["Material"] = MaterialBody;

	const std::string FileName = SanitizeFileName(MaterialName.empty() ? "Material" : MaterialName);
	const std::filesystem::path OutputPath = JsonOutputDirectory / "Material" / (FileName + ".json");

	SaveJsonToFile(MaterialFileJson, OutputPath);

	std::cout << "  -> Material JSON 저장 : " << OutputPath.string() << "\n";

	// StaticMesh json에는 머티리얼 이름만 참조로 남긴다.
	json MaterialRef;
	MaterialRef["Name"] = MaterialName;
	return MaterialRef;
}

json FbxToStaticMeshJsonTranslator::ExportMaterialTextures(const ufbx_material& Material, const std::filesystem::path& JsonOutputDirectory)
{
	json TextureArray = json::array();

	int Slot = 0;

	for (const ufbx_material_texture& MaterialTexture : Material.textures)
	{
		if (!MaterialTexture.texture)
		{
			continue;
		}

		const ufbx_texture& Texture = *MaterialTexture.texture;

		std::string TextureName =
			ConvertUfbxString(Texture.name);

		if (TextureName.empty())
		{
			const std::filesystem::path TextureFilePath =
				ConvertUfbxString(Texture.relative_filename);

			TextureName = TextureFilePath.stem().string();
		}

		if (TextureName.empty())
		{
			TextureName = "Texture_" + std::to_string(Slot);
		}

		// Material 이름을 결합하여 고유한 Texture Key 생성
		const std::string MaterialName =
			ConvertUfbxString(Material.name);

		TextureName =
			SanitizeFileName(MaterialName) + "_" +
			SanitizeFileName(TextureName);

		// 이름이 없으면 파일 경로의 파일명(확장자 제외)을 사용
		if (TextureName.empty())
		{
			const std::filesystem::path TextureFilePath = ConvertUfbxString(Texture.relative_filename);
			TextureName = TextureFilePath.stem().string();
		}

		// 그래도 없으면 슬롯 번호로 임시 이름 생성
		if (TextureName.empty())
		{
			TextureName = "Texture_" + std::to_string(Slot);
		}

		json TextureFileJson;
		TextureFileJson["Name"] = TextureName;
		TextureFileJson["Path"] = ConvertUfbxString(Texture.relative_filename);
		TextureFileJson["Type"] = "DDS";

		const std::string FileName = SanitizeFileName(TextureName);
		const std::filesystem::path OutputPath = JsonOutputDirectory / "Texture" / (FileName + ".json");

		SaveJsonToFile(TextureFileJson, OutputPath);

		std::cout << "  -> Texture JSON 저장 : " << OutputPath.string() << "\n";

		json TextureRef;
		TextureRef["Slot"] = Slot;
		TextureRef["Name"] = TextureName;

		TextureArray.push_back(TextureRef);

		++Slot;
	}

	return TextureArray;
}

json FbxToStaticMeshJsonTranslator::MakeColorJson(const ufbx_material_map& ColorMap, double DefaultR, double DefaultG, double DefaultB, double DefaultA)
{
	json Result;

	if (ColorMap.has_value)
	{
		Result["r"] = ColorMap.value_vec3.x;
		Result["g"] = ColorMap.value_vec3.y;
		Result["b"] = ColorMap.value_vec3.z;
		Result["a"] = DefaultA;
	}
	else
	{
		Result["r"] = DefaultR;
		Result["g"] = DefaultG;
		Result["b"] = DefaultB;
		Result["a"] = DefaultA;
	}

	return Result;
}

json FbxToStaticMeshJsonTranslator::MakeVec3Json(const ufbx_vec3& Vector)
{
	json Result;
	Result["x"] = Vector.x;
	Result["y"] = Vector.y;
	Result["z"] = Vector.z;
	return Result;
}

void FbxToStaticMeshJsonTranslator::SaveJsonToFile(const json& JsonData, const std::filesystem::path& FilePath)
{
	std::filesystem::create_directories(FilePath.parent_path());

	std::ofstream OutFile(FilePath);
	if (!OutFile.is_open())
	{
		std::cerr << "JSON 파일 저장 실패 : " << FilePath.string() << "\n";
		return;
	}

	// 사람이 읽기 좋게 4칸 들여쓰기로 저장
	OutFile << JsonData.dump(4);
}

std::string FbxToStaticMeshJsonTranslator::SanitizeFileName(const std::string& Name)
{
	static const std::string InvalidChars = "\\/:*?\"<>|";

	std::string Result = Name;

	for (char& Character : Result)
	{
		if (InvalidChars.find(Character) != std::string::npos)
		{
			Character = '_';
		}
	}

	if (Result.empty())
	{
		Result = "Unnamed";
	}

	return Result;
}

std::string FbxToStaticMeshJsonTranslator::ConvertUfbxString(const ufbx_string& String)
{
	if (!String.data || String.length == 0)
	{
		return {};
	}

	return std::string(String.data, String.length);
}
