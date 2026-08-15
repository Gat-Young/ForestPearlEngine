#include "FPAssetLoader.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include <fstream>
#include <iostream>
#include "Libraries/nlohmann/json.hpp"
#include "Utility/FPPathManager.h"
#include "Shader/ShaderFactory.h"
#include "Renderers/RenderingDevice.h"

using json = nlohmann::json;

void* FPAssetLoader::MakeVertexBuffer(std::vector<VERTEX> Mesh)
{
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(VERTEX));
}

void FPAssetLoader::LoadFbxData(std::string FbxPath, AssetOwner EngineAsset)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	//fbx Load 옵션 설정
	ufbx_load_opts Opts = {};
	Opts.target_axes = ufbx_axes_left_handed_y_up;
	Opts.target_unit_meters = 1.0f;

	ufbx_error Error;

	std::string FilePath = (FPPathManager::Get().GetAssetPath("Model/" + FbxPath));
	if (EngineAsset == AssetOwner::Engine) FilePath = (FPPathManager::Get().GetEngineAssetPath("Model/" + FbxPath));

	ufbx_scene* Scene = ufbx_load_file(FilePath.c_str(), &Opts, &Error);
	if (!Scene)
	{
		fprintf(stderr, "Failed to load Scene : %s\n", Error.description.data);
	}

	for (ufbx_node* Node : Scene->nodes)
	{
		if (!Node) { continue; }

		const std::string NodeName = ConvertUfbxString(Node->name);

		// 본, 카메라, 라이트, 빈 노드 등 메시가 없는 요소는 건너 뜀
		if (!Node->mesh) { continue; }

		const ufbx_mesh* Mesh = Node->mesh;

		//메시 정점 추출
		FPMeshData MeshData = ConvertUfbxMesh(Mesh, Node);

		if (MeshData.Name.empty())
		{
			MeshData.Name = NodeName;
		}

		AssetManager->AddMeshData(FbxPath, &MeshData);

		void* VertexBuffer = MakeVertexBuffer(AssetManager->GetMeshData(FbxPath).back().Vertices);
		int VBSize = AssetManager->GetMeshData(FbxPath).back().Vertices.size();

		//정점버퍼 생성 후 Map에 정보 등록
		AssetManager->AddVertexBuffer(FbxPath, VertexBuffer, VBSize, sizeof(VERTEX), 0);
	}
	ufbx_free_scene(Scene);
}

//ufbx_string -> std::string
std::string FPAssetLoader::ConvertUfbxString(ufbx_string String)
{
	if (!String.data || String.length == 0)
	{
		return {};
	}

	return std::string(String.data, String.length);
}

// ufbx_mesh -> FPMeshData
// 인덱스 버퍼를 생성하지 않음
// 삼각형 코너마다 정점을 하나씩 복사
FPMeshData FPAssetLoader::ConvertUfbxMesh(const ufbx_mesh* Mesh, const ufbx_node* Node)
{
	FPMeshData Result;

	if (!Mesh)
	{
		return Result;
	}

	Result.Name = ConvertUfbxString(Mesh->name);

	// 각 면을 삼각형화할 때 사용할 임시 코너 인덱스 배열
	// 삼각형 하나당 코너 인덱스 3개가 필요
	std::vector<uint32_t> TriangleCorners(Mesh->max_face_triangles * 3);

	//메모리 재할당 감소를 위해 대략적인 정점 메모리를 미리 예약
	Result.Vertices.reserve(Mesh->num_indices);

	//모든 면 순회
	for (const ufbx_face& Face : Mesh->faces)
	{
		//모두 삼각형으로 변환해 면에서 생성된 삼각형 개수 반환
		const uint32_t TriangleCount = ufbx_triangulate_face(TriangleCorners.data(), TriangleCorners.size(), Mesh, Face);

		//코너 개수
		const size_t CornerCount = static_cast<size_t>(TriangleCount) * 3;

		//삼각형 코너 마다 정점 생성
		for (size_t Corner = 0; Corner < CornerCount; ++Corner)
		{
			//ufbx에서 이 값은 단순 위치 정점 번호가 아니라, 면에서 사용하는 코너 인덱스
			//Position, Normal, UV, Color를 모두 같은 CornerIndex로 조회

			const uint32_t CornerIndex = TriangleCorners[Corner];

			//Position(Local)
			const ufbx_vec3 Position = ufbx_get_vertex_vec3(&Mesh->vertex_position, CornerIndex);

			//Position(World)
			const ufbx_vec3 WorldPosition = ufbx_transform_position(
				&Node->node_to_world,
				Position
			);
			//Normal
			ufbx_vec3 Normal = { 0.0f, 0.0f, 0.0f };

			if (Mesh->vertex_normal.exists)
			{
				Normal = ufbx_get_vertex_vec3(&Mesh->vertex_normal, CornerIndex);
			}

			//UV
			ufbx_vec2 UV = { 0.0f, 0.0f };

			if (Mesh->vertex_uv.exists)
			{
				UV = ufbx_get_vertex_vec2(&Mesh->vertex_uv, CornerIndex);
			}

			//VertexColor
			ufbx_vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

			if (Mesh->vertex_color.exists)
			{
				Color = ufbx_get_vertex_vec4(&Mesh->vertex_color, CornerIndex);
			}

			//엔진 정점으로 복사
			VERTEX Vertex{};
			Vertex.x = static_cast<float>(WorldPosition.x);
			Vertex.y = static_cast<float>(WorldPosition.y);
			Vertex.z = static_cast<float>(WorldPosition.z);

			Vertex.r = static_cast<float>(Color.x);
			Vertex.g = static_cast<float>(Color.y);
			Vertex.b = static_cast<float>(Color.z);
			Vertex.a = static_cast<float>(Color.w);

			Result.Vertices.push_back(Vertex);
		}
	}



	return Result;
}

//Level 정보를 저장
void FPAssetLoader::LoadLevelData(std::string LevelName, std::string LevelPath, AssetOwner EngineAsset)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	if (AssetManager->HasLevelData(LevelName))
	{
		std::cout << "같은 이름의 Level이 존재합니다." << "\n";
		return;
	}

	std::ifstream File(FPPathManager::Get().GetAssetPath("Level/" + LevelPath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("Level/" + LevelPath));

	if (!File.is_open())
	{
		std::cerr << "Level.json 파일 열기 실패\n";
		return;
	}

	json JsonLevelData;

	try
	{
		File >> JsonLevelData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	std::string GameModeName = JsonLevelData["gamemode"];
	AssetManager->AddGameModeData(LevelName, GameModeName);

	for (const json& ActorData : JsonLevelData["actors"])
	{
		std::string ClassName = ActorData["class"];
		std::string ActorName = ActorData["name"];

		const json& Transform = ActorData["transform"];
		const json& Location = Transform["location"];
		const json& Rotation = Transform["rotation"];
		const json& Scale = Transform["scale"];

		float LocationX = Location["x"];
		float LocationY = Location["y"];
		float LocationZ = Location["z"];

		float RotationX = Rotation["x"];
		float RotationY = Rotation["y"];
		float RotationZ = Rotation["z"];

		float ScaleX = Scale["x"];
		float ScaleY = Scale["y"];
		float ScaleZ = Scale["z"];

		FPActorData Data;

		Data.ClassName = ClassName;
		Data.ActorName = ActorName;

		Data.Location_x = LocationX;
		Data.Location_y = LocationY;
		Data.Location_z = LocationZ;

		Data.Rotation_x = RotationX;
		Data.Rotation_y = RotationY;
		Data.Rotation_z = RotationZ;

		Data.Scale_x = ScaleX;
		Data.Scale_y = ScaleY;
		Data.Scale_Z = ScaleZ;

		AssetManager->AddLevelData(LevelName, &Data);
	}
}

void FPAssetLoader::LoadVertexShader(std::string ShaderObjPath, AssetOwner EngineAsset)
{
	std::wstring FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetAssetShaderPath(ShaderObjPath)));
	if (EngineAsset == AssetOwner::Engine) FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetEngineAssetShaderPath(ShaderObjPath)));

	void* VertexShader = nullptr;
	void* VSCode = nullptr;

	ShaderFactory::GetShaderFactory().VertexShaderLoad(FilePath.c_str(), &VertexShader, &VSCode);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	AssetManager->AddVertexShader(ShaderObjPath, VertexShader, VSCode);
}

void FPAssetLoader::LoadPixelShader(std::string ShaderObjPath, AssetOwner EngineAsset)
{
	std::wstring FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetAssetShaderPath(ShaderObjPath)));
	if (EngineAsset == AssetOwner::Engine) FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetEngineAssetShaderPath(ShaderObjPath)));

	void* PixelShader = nullptr;
	void* PSCode = nullptr;

	ShaderFactory::GetShaderFactory().PixelShaderLoad(FilePath.c_str(), &PixelShader, &PSCode);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	AssetManager->AddPixelShader(ShaderObjPath, PixelShader, PSCode);
}

void FPAssetLoader::LoadVertexShader(std::string ShaderPath, std::string VS_Main, std::string ShaderModel, AssetOwner EngineAsset)
{
	std::wstring FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetAssetPath("Shader/" + ShaderPath)));
	if (EngineAsset == AssetOwner::Engine) FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetEngineAssetPath("Shader/" + ShaderPath)));

	void* VertexShader = nullptr;
	void* VSCode = nullptr;

	ShaderFactory::GetShaderFactory().VertexShaderLoad(FilePath.c_str(), VS_Main.c_str(), ShaderModel.c_str(), &VertexShader, &VSCode);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	AssetManager->AddVertexShader(ShaderPath, VertexShader, VSCode);
}

void FPAssetLoader::LoadPixelShader(std::string ShaderPath, std::string PS_Main, std::string ShaderModel, AssetOwner EngineAsset)
{
	std::wstring FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetAssetPath("Shader/" + ShaderPath)));
	if (EngineAsset == AssetOwner::Engine) FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetEngineAssetPath("Shader/" + ShaderPath)));

	void* PixelShader = nullptr;
	void* PSCode = nullptr;

	ShaderFactory::GetShaderFactory().PixelShaderLoad(FilePath.c_str(), PS_Main.c_str(), ShaderModel.c_str(), &PixelShader, &PSCode);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	AssetManager->AddPixelShader(ShaderPath, PixelShader, PSCode);
}
