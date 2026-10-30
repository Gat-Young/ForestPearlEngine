#include "FbxToStaticMeshJsonTranslator.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <unordered_map>

using json = nlohmann::ordered_json;

namespace
{
	// 확장자(점 제외, 소문자)를 기준으로 엔진 텍스처 로더 타입을 결정한다. (AssetLoader::LoadTexture 참고)
	std::string DetermineTextureType(const std::string& LowerExtensionNoDot)
	{
		return LowerExtensionNoDot == "dds" ? "DDS" : "WIC";
	}

	// FBX 텍스처의 Wrap 설정을 엔진의 FPSamplerType 문자열로 변환한다. (Renderers/FPRenderingCommon.h 참고)
	// ufbx는 Repeat/Clamp 두 가지만 구분하므로 그 외 Sampler_Mirror 등은 사용하지 않는다.
	// U/V 축 설정이 다를 수 있지만 엔진 쪽 텍스처 슬롯에는 SamplerType이 하나뿐이라 U축(wrap_u) 기준으로 결정한다.
	std::string DetermineSamplerType(ufbx_wrap_mode WrapU)
	{
		return WrapU == UFBX_WRAP_REPEAT ? "Sampler_Wrap" : "Sampler_CLAMP";
	}

	// 문자열의 첫 글자만 대문자로 바꾼다. (예: "hair" -> "Hair")
	std::string CapitalizeFirstLetter(const std::string& Text)
	{
		if (Text.empty())
		{
			return Text;
		}

		std::string Result = Text;
		Result[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(Result[0])));
		return Result;
	}

	// 확장자를 소문자로 변환한다. (점 제외)
	std::string ToLowerExtensionNoDot(const std::filesystem::path& FilePath)
	{
		std::string Extension = FilePath.extension().string();
		if (!Extension.empty() && Extension.front() == '.')
		{
			Extension.erase(Extension.begin());
		}

		std::transform(Extension.begin(), Extension.end(), Extension.begin(), [](unsigned char Character) { return static_cast<char>(std::tolower(Character)); });
		return Extension;
	}
}

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

	// 입력 폴더 전체(여러 FBX 파일)에서 공유되는, 이미 저장한 기본 Material / Texture 이름 집합.
	// 같은 이름의 머티리얼/텍스처가 여러 메쉬에서 재사용되어도 파일을 중복으로 만들지 않기 위함.
	std::set<std::string> ExportedMaterialNames;
	std::set<std::string> ExportedTextureNames;

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

		if (TranslateFbxFile(Entry.path(), JsonOutputDirectory, ExportedMaterialNames, ExportedTextureNames))
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

bool FbxToStaticMeshJsonTranslator::TranslateFbxFile(const std::filesystem::path& FbxFilePath, const std::filesystem::path& JsonOutputDirectory, std::set<std::string>& ExportedMaterialNames, std::set<std::string>& ExportedTextureNames)
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

	// 엔진 AssetLoader가 "Model/<FbxPath>"로 찾아갈 수 있도록, "<fbx파일이름>/<fbx파일이름>.fbx" 형태의 상대 경로를 만든다.
	// 예 : ToonLink.fbx -> "ToonLink/ToonLink.fbx"
	const std::string FbxFileName = FbxFilePath.filename().string();
	const std::string FbxFileStem = FbxFilePath.stem().string();
	const std::string MeshDataPath = FbxFileStem + "/" + FbxFileName;

	bool bAnyMeshExported = false;

	// 씬의 모든 노드를 순회하며 메쉬를 가진 노드만 StaticMesh json으로 변환
	for (ufbx_node* Node : Scene->nodes)
	{
		if (!Node || !Node->mesh)
		{
			continue;
		}

		const ufbx_mesh& Mesh = *Node->mesh;

		// 파일/필드 이름에 쓸 StaticMesh 이름 : NodeName + "_StaticMesh" (추후 더 보기 좋은 이름으로 수동 치환 가능)
		std::string NodeName = ConvertUfbxString(Node->name);
		if (NodeName.empty())
		{
			NodeName = ConvertUfbxString(Mesh.name);
		}
		if (NodeName.empty())
		{
			NodeName = FbxFileStem;
		}

		const std::string StaticMeshName = SanitizeFileName(NodeName) + "_StaticMesh";

		// 자식 로케이터 노드 -> 소켓 목록
		const json SocketsJson = BuildSocketsJson(*Node);

		// 머티리얼 -> 기본 Material.json(재질당 1회) + Material Instance json(메쉬당 1회) 생성 및 참조 정보
		const json MaterialJson = ExportMeshMaterial(Mesh, StaticMeshName, JsonOutputDirectory, ExportedMaterialNames, ExportedTextureNames);

		const json StaticMeshJson = BuildStaticMeshJson(*Node, StaticMeshName, MeshDataPath, SocketsJson, MaterialJson);

		const std::filesystem::path OutputPath = JsonOutputDirectory / "StaticMesh" / (StaticMeshName + ".json");
		SaveJsonToFile(StaticMeshJson, OutputPath);

		std::cout << "  -> StaticMesh JSON 저장 : " << OutputPath.string() << "\n";

		bAnyMeshExported = true;
	}

	ufbx_free_scene(Scene);

	return bAnyMeshExported;
}

json FbxToStaticMeshJsonTranslator::BuildStaticMeshJson(const ufbx_node& Node, const std::string& StaticMeshName, const std::string& MeshDataPath, const json& SocketsJson, const json& MaterialJson)
{
	json Result;

	Result["Name"] = StaticMeshName;
	Result["NodeName"] = ConvertUfbxString(Node.name);

	// 실제 정점/인덱스 버퍼 리소스는 이 json이 아니라 엔진이 FBX에서 직접 읽어오므로,
	// Path에는 엔진 AssetLoader가 찾아갈 원본 FBX의 상대 경로를 채운다.
	json MeshDataEntry;
	MeshDataEntry["Path"] = MeshDataPath;
	MeshDataEntry["Topology"] = "TRIANGLELIST";

	Result["MeshData"] = json::array({ MeshDataEntry });
	Result["Sockets"] = SocketsJson;
	Result["Material"] = MaterialJson;

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

json FbxToStaticMeshJsonTranslator::ExportMeshMaterial(const ufbx_mesh& Mesh, const std::string& StaticMeshName, const std::filesystem::path& JsonOutputDirectory, std::set<std::string>& ExportedMaterialNames, std::set<std::string>& ExportedTextureNames)
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

	const json BaseColorArray = MakeColorJson(Material.pbr.base_color, 1.0, 1.0, 1.0, 1.0);
	const json DiffuseArray = MakeColorJson(Material.fbx.diffuse_color, 1.0, 1.0, 1.0, 1.0);
	const json AmbientArray = MakeColorJson(Material.fbx.ambient_color, 0.1, 0.1, 0.1, 1.0);

	// Emissive(자체 발광)는 FBX 값과 무관하게 항상 0으로 시작한다. (필요하면 추후 머티리얼 에디터에서 수동으로 조절)
	const json EmissiveArray = MakeColorJson({}, 0.0, 0.0, 0.0, 0.0);

	// 엔진이 항상 읽는 MaterialConstBuffer 파라미터 블록. Shader Reflection은 하지 않으므로
	// 다른 상수버퍼(UVTransformCB 등)는 만들지 않고 이 고정 블록만 생성한다. (SimpleMaterial.json 참고)
	json MaterialConstBufferValues;
	MaterialConstBufferValues["Material.BaseColor"] = BaseColorArray;
	MaterialConstBufferValues["Material.Diffuse"] = DiffuseArray;
	MaterialConstBufferValues["Material.Ambient"] = AmbientArray;
	MaterialConstBufferValues["Material.Emissive"] = EmissiveArray;

	json MaterialConstBufferParameter;
	MaterialConstBufferParameter["Type"] = "VS";
	MaterialConstBufferParameter["ConstantBuffer"] = "MaterialConstBuffer";
	MaterialConstBufferParameter["Values"] = MaterialConstBufferValues;

	json ParametersArray = json::array();
	ParametersArray.push_back(MaterialConstBufferParameter);

	// FBX의 Two Sided(양면) 설정. 엔진 FPMaterial::TwoSided와 동일한 의미(체크 시 Cull 없이 Back Normal도 사용).
	const bool bTwoSided = Material.features.double_sided.enabled;

	// 기본 Material : 재질당 한 번만 저장 (Parameters만, Texture/Sampler 없음)
	const std::string BaseMaterialName = SanitizeFileName(MaterialName.empty() ? "Material" : MaterialName) + "_Material";

	if (ExportedMaterialNames.insert(BaseMaterialName).second)
	{
		json BaseMaterialBody;
		BaseMaterialBody["Name"] = BaseMaterialName;
		BaseMaterialBody["VertexShader"] = "DefaultVertexShader";
		BaseMaterialBody["PixelShader"] = "DefaultPixelShader";
		BaseMaterialBody["TwoSided"] = bTwoSided;
		BaseMaterialBody["Parameters"] = ParametersArray;

		json BaseMaterialFileJson;
		BaseMaterialFileJson["Material"] = BaseMaterialBody;

		const std::filesystem::path BaseMaterialPath = JsonOutputDirectory / "Material" / (BaseMaterialName + ".json");
		SaveJsonToFile(BaseMaterialFileJson, BaseMaterialPath);

		std::cout << "  -> Material JSON 저장 : " << BaseMaterialPath.string() << "\n";
	}

	// Material Instance : 이 메쉬 전용. 기본 머티리얼과 같은 Parameters + 이 메쉬가 쓰는 Textures/Samplers 배열을 포함한다.
	json TexturesArray;
	json SamplersArray;
	ExportMaterialTextures(Material, JsonOutputDirectory, ExportedTextureNames, TexturesArray, SamplersArray);

	const std::string MaterialInstanceName = SanitizeFileName(MaterialName.empty() ? "Material" : MaterialName) + "_" + StaticMeshName + "_MaterialInstance";

	json MaterialInstanceBody;
	MaterialInstanceBody["Name"] = MaterialInstanceName;
	MaterialInstanceBody["VertexShader"] = "DefaultVertexShader";
	MaterialInstanceBody["PixelShader"] = "DefaultPixelShader";
	MaterialInstanceBody["TwoSided"] = bTwoSided;
	MaterialInstanceBody["Parameters"] = ParametersArray;
	MaterialInstanceBody["Textures"] = TexturesArray;
	MaterialInstanceBody["Samplers"] = SamplersArray;

	json MaterialInstanceFileJson;
	MaterialInstanceFileJson["Material"] = MaterialInstanceBody;

	const std::filesystem::path MaterialInstancePath = JsonOutputDirectory / "Material" / (MaterialInstanceName + ".json");
	SaveJsonToFile(MaterialInstanceFileJson, MaterialInstancePath);

	std::cout << "  -> Material Instance JSON 저장 : " << MaterialInstancePath.string() << "\n";

	// StaticMesh json에는 Material Instance 이름만 참조로 남긴다.
	json MaterialRef;
	MaterialRef["Name"] = MaterialInstanceName;
	return MaterialRef;
}

void FbxToStaticMeshJsonTranslator::ExportMaterialTextures(const ufbx_material& Material, const std::filesystem::path& JsonOutputDirectory, std::set<std::string>& ExportedTextureNames, nlohmann::ordered_json& OutTextures, nlohmann::ordered_json& OutSamplers)
{
	OutTextures = json::array();
	OutSamplers = json::array();

	// 같은 ufbx 속성(material_prop)을 여러 텍스처가 공유하는 경우(예: 레이어 텍스처)에만 ArrayIndex를 붙인다.
	std::unordered_map<std::string, int> ParameterUseCount;
	for (const ufbx_material_texture& MaterialTexture : Material.textures)
	{
		if (!MaterialTexture.texture)
		{
			continue;
		}

		++ParameterUseCount[ConvertUfbxString(MaterialTexture.material_prop)];
	}

	std::unordered_map<std::string, int> ParameterNextIndex;
	int Slot = 0;

	for (const ufbx_material_texture& MaterialTexture : Material.textures)
	{
		if (!MaterialTexture.texture)
		{
			continue;
		}

		const ufbx_texture& Texture = *MaterialTexture.texture;

		// 실제 이미지 파일명을 기준으로 이름을 만든다 (예: "hair.png" -> "Hair_png").
		// 같은 파일을 가리키는 슬롯(Base/Alpha 등)이 여러 개여도 Texture json은 한 번만 생성하고 이름만 재사용한다.
		std::filesystem::path TextureFilePath = ConvertUfbxString(Texture.relative_filename);
		if (TextureFilePath.empty())
		{
			TextureFilePath = ConvertUfbxString(Texture.filename);
		}

		std::string TextureFileName = TextureFilePath.filename().string();
		std::string TextureStem = TextureFilePath.stem().string();
		std::string ExtensionNoDot = ToLowerExtensionNoDot(TextureFilePath);

		std::string TextureName;

		if (!TextureStem.empty() && !ExtensionNoDot.empty())
		{
			TextureName = CapitalizeFirstLetter(TextureStem) + "_" + ExtensionNoDot;
		}
		else if (!TextureStem.empty())
		{
			TextureName = CapitalizeFirstLetter(TextureStem);
		}
		else
		{
			// 파일 경로 정보가 전혀 없으면 ufbx 텍스처 이름이나 슬롯 번호로 대체
			TextureName = ConvertUfbxString(Texture.name);
			if (TextureName.empty())
			{
				TextureName = "Texture_" + std::to_string(Slot);
			}
		}

		TextureName = SanitizeFileName(TextureName);

		if (ExportedTextureNames.insert(TextureName).second)
		{
			json TextureFileJson;
			TextureFileJson["Name"] = TextureName;
			TextureFileJson["Path"] = TextureFileName;
			TextureFileJson["Type"] = DetermineTextureType(ExtensionNoDot);

			const std::filesystem::path OutputPath = JsonOutputDirectory / "Texture" / (TextureName + ".json");
			SaveJsonToFile(TextureFileJson, OutputPath);

			std::cout << "  -> Texture JSON 저장 : " << OutputPath.string() << "\n";
		}

		// Shader Reflection은 하지 않으므로 실제 셰이더 변수 이름은 알 수 없다.
		// 대신 ufbx가 알려주는 FBX 속성 이름(material_prop, 예: DiffuseColor)을 그대로 Parameter로 사용한다.
		std::string ParameterName = ConvertUfbxString(MaterialTexture.material_prop);
		if (ParameterName.empty())
		{
			ParameterName = "Texture_" + std::to_string(Slot);
		}

		const bool bSharedParameter = ParameterUseCount[ParameterName] > 1;
		const int ArrayIndex = ParameterNextIndex[ParameterName]++;

		json TextureRef;
		TextureRef["Type"] = "PS";
		TextureRef["Parameter"] = ParameterName;
		if (bSharedParameter)
		{
			TextureRef["ArrayIndex"] = ArrayIndex;
		}
		TextureRef["Asset"] = TextureName;
		// 텍스처 리소스의 셰이더 바인딩 타입 (Texture2D/TextureCube 등). FBX에서 구분할 수 없어 기본값으로 고정한다.
		TextureRef["TextureType"] = "Texture2D";

		OutTextures.push_back(TextureRef);

		json SamplerRef;
		SamplerRef["Type"] = "PS";
		SamplerRef["Parameter"] = ParameterName;
		if (bSharedParameter)
		{
			SamplerRef["ArrayIndex"] = ArrayIndex;
		}
		// FBX에는 필터링 방식(Point/Bi-Linear/Tri-Linear/Anistropy) 정보가 없어 기본값으로 고정한다.
		// 엔진 StringToSamplerType이 그대로 찾을 수 있도록 "Filter_Tri_Linear_" + Wrap 설정을 합친 문자열로 저장한다. (FPRenderingCommon.cpp 참고)
		SamplerRef["SamplerType"] = "Filter_Tri_Linear_" + DetermineSamplerType(Texture.wrap_u);

		OutSamplers.push_back(SamplerRef);

		++Slot;
	}
}

json FbxToStaticMeshJsonTranslator::MakeColorJson(const ufbx_material_map& ColorMap, double DefaultR, double DefaultG, double DefaultB, double DefaultA)
{
	if (ColorMap.has_value)
	{
		return json::array({ ColorMap.value_vec3.x, ColorMap.value_vec3.y, ColorMap.value_vec3.z, DefaultA });
	}

	return json::array({ DefaultR, DefaultG, DefaultB, DefaultA });
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
