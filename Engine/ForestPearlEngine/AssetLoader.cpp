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

void FPAssetLoader::MakeGizmoVertexBuffer(std::vector<struct VERTEX>& Mesh, const std::string& GizemoMeshPath)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	if (AssetManager->HasStaticMeshData(GizemoMeshPath))
	{
		std::cout << "이미 존재 하는 메시 데이터 입니다!" << "\n";
		return;
	}

	FPMeshData MeshData;
	MeshData.Name = GizemoMeshPath;

	//Gizmo가 필요한 정보만 모아서 Vertex Buffer를 생성
	std::vector<POSITION_VERTEX> PositionVertex;
	std::vector<COLOR_VERTEX> ColorVertex;

	for (VERTEX Vertex : Mesh)
	{
		PositionVertex.push_back(Vertex.PositionVertex);
		ColorVertex.push_back(Vertex.ColorVertex);
	}
	
	MeshData.Position_Vertices = PositionVertex;
	MeshData.Color_Vertices = ColorVertex;

	//Gizmo Mesh 정보를 저장
	AssetManager->AddMeshData(GizemoMeshPath, &MeshData);

	void* PositionVB = MakePositionVertexBuffer(PositionVertex);
	void* ColorVB = MakeColorVertexBuffer(ColorVertex);

	//정점버퍼 생성 후 Map에 정보 등록 (Position , Color)
	AssetManager->AddPositionVertexBuffer(GizemoMeshPath, PositionVB, PositionVertex.size() * sizeof(POSITION_VERTEX), PositionVertex.size(), sizeof(POSITION_VERTEX), 0);
	AssetManager->AddColorVertexBuffer(GizemoMeshPath, ColorVB, ColorVertex.size() * sizeof(COLOR_VERTEX), ColorVertex.size(), sizeof(COLOR_VERTEX), 0);
}

void* FPAssetLoader::MakePositionVertexBuffer(std::vector<struct POSITION_VERTEX>& Mesh)
{
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(POSITION_VERTEX));
}

void* FPAssetLoader::MakeColorVertexBuffer(std::vector<struct COLOR_VERTEX>& Mesh)
{
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(COLOR_VERTEX));
}

void* FPAssetLoader::MakeNormalVertexBuffer(std::vector<struct NORMAL_VERTEX>& Mesh)
{
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(NORMAL_VERTEX));
}

void* FPAssetLoader::MakeUVVertexBuffer(std::vector<struct UV_VERTEX>& Mesh)
{
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(UV_VERTEX));
}

void* FPAssetLoader::MakeIndex16BitBuffer(std::vector<struct INDEX_16BIT>& Mesh)
{
	return RenderingDevice::GetRenderingDevice().CreateIndexBuffer(Mesh.data(), Mesh.size(), sizeof(INDEX_16BIT));
}

void* FPAssetLoader::MakeIndex32BitBuffer(std::vector<struct INDEX_32BIT>& Mesh)
{
	return RenderingDevice::GetRenderingDevice().CreateIndexBuffer(Mesh.data(), Mesh.size(), sizeof(INDEX_32BIT));
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

		//각각에 맞는 버퍼 데이터를 만들어서 등록
		FPVertexBufferData PositionVertex;
		PositionVertex.VertexBuffer = MakePositionVertexBuffer(MeshData.Position_Vertices);
		PositionVertex.VB_Cnt = MeshData.Position_Vertices.size();
		PositionVertex.VB_Size = MeshData.Position_Vertices.size() * sizeof(POSITION_VERTEX);
		PositionVertex.Stride = sizeof(POSITION_VERTEX);
		PositionVertex.Offset = 0;

		FPVertexBufferData NormalVertex;
		NormalVertex.VertexBuffer = MakeNormalVertexBuffer(MeshData.Normal_Vertices);
		NormalVertex.VB_Cnt = MeshData.Normal_Vertices.size();
		NormalVertex.VB_Size = MeshData.Normal_Vertices.size() * sizeof(NORMAL_VERTEX);
		NormalVertex.Stride = sizeof(NORMAL_VERTEX);
		NormalVertex.Offset = 0;

		FPVertexBufferData ColorVertex;
		ColorVertex.VertexBuffer = MakeColorVertexBuffer(MeshData.Color_Vertices);
		ColorVertex.VB_Cnt = MeshData.Color_Vertices.size();
		ColorVertex.VB_Size = MeshData.Color_Vertices.size() * sizeof(COLOR_VERTEX);
		ColorVertex.Stride = sizeof(COLOR_VERTEX);
		ColorVertex.Offset = 0;

		FPVertexBufferData UVVertex;
		UVVertex.VertexBuffer = MakeUVVertexBuffer(MeshData.UV_Vertices);
		UVVertex.VB_Cnt = MeshData.UV_Vertices.size();
		UVVertex.VB_Size = MeshData.UV_Vertices.size() * sizeof(UV_VERTEX);
		UVVertex.Stride = sizeof(UV_VERTEX);
		UVVertex.Offset = 0;

		FPIndexBufferData IndexBuffer;
		IndexBuffer.IndexBuffer = MakeIndex32BitBuffer(MeshData.Index);
		IndexBuffer.Index_Size = MeshData.Index.size() * sizeof(INDEX_32BIT);
		IndexBuffer.Index_Cnt = MeshData.Index.size() * 3;
		IndexBuffer.FaceCnt = MeshData.Index.size();
		IndexBuffer.Stride = sizeof(INDEX_32BIT);
		IndexBuffer.Offset = 0;
		
		AssetManager->AddPositionVertexBuffer(FbxPath , PositionVertex);
		AssetManager->AddNormalVertexBuffer(FbxPath, NormalVertex);
		AssetManager->AddColorVertexBuffer(FbxPath, ColorVertex);
		AssetManager->AddUVVertexBuffer(FbxPath, UVVertex);
		AssetManager->AddIndexBuffer(FbxPath, IndexBuffer);

		AssetManager->AddMeshData(FbxPath, &MeshData);

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

	// 삼각형화 이후 예상되는 정점 개수
	const size_t VertexCount = Mesh->num_triangles * 3;

	//메모리 재할당 감소를 위해 대략적인 정점 메모리를 미리 예약
	Result.Position_Vertices.reserve(VertexCount);
	Result.Normal_Vertices.reserve(VertexCount);
	Result.Color_Vertices.reserve(VertexCount);
	Result.UV_Vertices.reserve(VertexCount);

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
				const ufbx_vec3 LocalNormal =
					ufbx_get_vertex_vec3(
						&Mesh->vertex_normal,
						CornerIndex
					);

				Normal = ufbx_transform_direction(
					&Node->node_to_world,
					LocalNormal
				);

				// Normalize
				const double Length = std::sqrt(
					Normal.x * Normal.x +
					Normal.y * Normal.y +
					Normal.z * Normal.z
				);

				if (Length > 0.0)
				{
					Normal.x /= Length;
					Normal.y /= Length;
					Normal.z /= Length;
				}
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
			POSITION_VERTEX PositionVertex{};

			PositionVertex.x = static_cast<float>(WorldPosition.x);
			PositionVertex.y = static_cast<float>(WorldPosition.y);
			PositionVertex.z = static_cast<float>(WorldPosition.z);

			Result.Position_Vertices.push_back(PositionVertex);

			NORMAL_VERTEX NormalVertex{};

			NormalVertex.nx = static_cast<float>(Normal.x);
			NormalVertex.ny = static_cast<float>(Normal.y);
			NormalVertex.nz = static_cast<float>(Normal.z);

			Result.Normal_Vertices.push_back(NormalVertex);

			UV_VERTEX UVVertex{};

			UVVertex.u = static_cast<float>(UV.x);
			UVVertex.v = static_cast<float>(UV.y);

			Result.UV_Vertices.push_back(UVVertex);

			COLOR_VERTEX ColorVertex{};

			ColorVertex.r = static_cast<float>(Color.x);
			ColorVertex.g = static_cast<float>(Color.y);
			ColorVertex.b = static_cast<float>(Color.z);
			ColorVertex.a = static_cast<float>(Color.w);

			Result.Color_Vertices.push_back(ColorVertex);

		}
	}

	// Index Buffer 생성
	const size_t TotalVertexCount = Result.Position_Vertices.size();

	if (TotalVertexCount == 0)
	{
		return Result;
	}

	// ufbx_generate_indices()는 memcmp()로 비교하므로
	// 정점 구조체에 패딩 바이트가 없어야 함
	static_assert(sizeof(POSITION_VERTEX) == 12);
	static_assert(sizeof(NORMAL_VERTEX) == 12);
	static_assert(sizeof(COLOR_VERTEX) == 16);
	static_assert(sizeof(UV_VERTEX) == 8);

	// 각 속성을 독립된 Vertex Stream으로 등록
	ufbx_vertex_stream VertexStreams[] =
	{
		{
			Result.Position_Vertices.data(),
			TotalVertexCount,
			sizeof(POSITION_VERTEX)
		},
		{
			Result.Normal_Vertices.data(),
			TotalVertexCount,
			sizeof(NORMAL_VERTEX)
		},
		{
			Result.Color_Vertices.data(),
			TotalVertexCount,
			sizeof(COLOR_VERTEX)
		},
		{
			Result.UV_Vertices.data(),
			TotalVertexCount,
			sizeof(UV_VERTEX)
		}
	};

	// 임시 Index 배열
	std::vector<uint32_t> Indices(TotalVertexCount);

	ufbx_error Error{};

	// 중복 정점 제거
	const size_t UniqueVertexCount = ufbx_generate_indices(
		VertexStreams,
		4,
		Indices.data(),
		Indices.size(),
		nullptr,
		&Error
	);


	// 실패 처리
	if (Error.type != UFBX_ERROR_NONE ||
		UniqueVertexCount == 0)
	{
		Result.Position_Vertices.clear();
		Result.Normal_Vertices.clear();
		Result.Color_Vertices.clear();
		Result.UV_Vertices.clear();

		return Result;
	}

	// --------------------------------------------------------
	// 3. 고유 정점 개수만큼 축소
	// --------------------------------------------------------

	Result.Position_Vertices.resize(UniqueVertexCount);
	Result.Normal_Vertices.resize(UniqueVertexCount);
	Result.Color_Vertices.resize(UniqueVertexCount);
	Result.UV_Vertices.resize(UniqueVertexCount);

	// ========================================================
	// 4. INDEX_32BIT 구조체로 변환
	// ========================================================

	static_assert(sizeof(unsigned long) == sizeof(uint32_t));

	const size_t FaceCount = Indices.size() / 3;

	Result.Index.reserve(FaceCount);

	for (size_t i = 0; i < Indices.size(); i += 3)
	{
		INDEX_32BIT Index{};

		Index.A = static_cast<unsigned long>(Indices[i]);
		Index.B = static_cast<unsigned long>(Indices[i + 1]);
		Index.C = static_cast<unsigned long>(Indices[i + 2]);

		Result.Index.push_back(Index);
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
		std::cerr << LevelName << ".json 파일 열기 실패\n";
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
	void* VBLayout = nullptr;

	ShaderFactory::GetShaderFactory().VertexShaderLoad(FilePath.c_str(), &VertexShader, &VSCode, &VBLayout);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	AssetManager->AddVertexShader(ShaderObjPath, VertexShader, VSCode, VBLayout);
}

void FPAssetLoader::LoadNormalDebugVertexShader(std::string ShaderObjPath, AssetOwner EngineAsset)
{
	std::wstring FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetAssetShaderPath(ShaderObjPath)));
	if (EngineAsset == AssetOwner::Engine) FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetEngineAssetShaderPath(ShaderObjPath)));

	void* VertexShader = nullptr;
	void* VSCode = nullptr;
	void* VBLayout = nullptr;

	ShaderFactory::GetShaderFactory().NormalVertexShaderLoad(FilePath.c_str(), &VertexShader, &VSCode, &VBLayout);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	AssetManager->AddVertexShader(ShaderObjPath, VertexShader, VSCode, VBLayout);
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
	void* VBLayout = nullptr;

	ShaderFactory::GetShaderFactory().VertexShaderLoad(FilePath.c_str(), VS_Main.c_str(), ShaderModel.c_str(), &VertexShader, &VSCode);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	AssetManager->AddVertexShader(ShaderPath, VertexShader, VSCode, VBLayout);
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

void FPAssetLoader::LoadStaticMesh(std::string StaticMeshName, std::string StaticMeshPath, AssetOwner EngineAsset)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	if (AssetManager->HasStaticMeshData(StaticMeshName))
	{
		std::cout << "같은 이름의 StaticMesh가 존재합니다." << "\n";
		return;
	}

	std::ifstream File(FPPathManager::Get().GetAssetPath("StaticMesh/" + StaticMeshPath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("StaticMesh/" + StaticMeshPath));

	if (!File.is_open())
	{
		std::cerr << StaticMeshName << ".json 파일 열기 실패\n";
		return;
	}

	json JsonStaticMeshData;

	try
	{
		File >> JsonStaticMeshData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	FPStaticMeshData Data;

	for (const json& MeshData : JsonStaticMeshData["MeshData"])
	{
		Data.MeshPath = MeshData["Path"];
		Data.MeshTopology = MeshData["Topology"];
	}

	if (JsonStaticMeshData.contains("Sockets") && JsonStaticMeshData["Sockets"].is_array())
	{
		if (!JsonStaticMeshData["Sockets"].empty())
		{
			for (const json& SocketData : JsonStaticMeshData["Sockets"])
			{
				std::string SocketName = SocketData["Name"];

				const json& Transform = SocketData["Transform"];
				const json& Location = Transform["Location"];
				const json& Rotation = Transform["Rotation"];
				const json& Scale = Transform["Scale"];

				float LocationX = Location["x"];
				float LocationY = Location["y"];
				float LocationZ = Location["z"];

				float RotationX = Rotation["x"];
				float RotationY = Rotation["y"];
				float RotationZ = Rotation["z"];

				float ScaleX = Scale["x"];
				float ScaleY = Scale["y"];
				float ScaleZ = Scale["z"];

				SOCKET_TRANSFORM SocketTransform;
				SocketTransform.SocketName = SocketName;
				SocketTransform.Location_x = LocationX;
				SocketTransform.Location_y = LocationY;
				SocketTransform.Location_z = LocationZ;

				SocketTransform.Rotation_x = RotationX;
				SocketTransform.Rotation_y = RotationY;
				SocketTransform.Rotation_z = RotationZ;

				SocketTransform.Scale_x = ScaleX;
				SocketTransform.Scale_y = ScaleY;
				SocketTransform.Scale_Z = ScaleZ;

				Data.Sockets.push_back(SocketTransform);
			}
		}
	}


	//Material 값 일단 Maapping해서 들고 있기
	std::string Material;

	if (JsonStaticMeshData.contains("Material") && JsonStaticMeshData["Material"].is_string())
	{
		Material = JsonStaticMeshData["Material"].get<std::string>();

		if (Material.empty())
		{
			// Material 없음
			Material.clear();
		}
	}

	Data.MaterialName = JsonStaticMeshData["Material"];

	AssetManager->AddStaticMeshData(StaticMeshName, &Data);
}

void FPAssetLoader::LoadTexture(std::string TextureName, std::string TexturePath, AssetOwner EngineAsset)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	if (AssetManager->HasTextureData(TextureName))
	{
		std::cout << "같은 이름의 Texture가 존재합니다." << "\n";
		return;
	}

	std::ifstream File(FPPathManager::Get().GetAssetPath("Texture/" + TexturePath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("Texture/" + TexturePath));

	if (!File.is_open())
	{
		std::cerr << TextureName << ".json 파일 열기 실패\n";
		return;
	}

	json JsonStaticMeshData;

	try
	{
		File >> JsonStaticMeshData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	FPTextureData Data;
	std::string TextureResourcePath;
	std::string TextureType;

	for (const json& TextureData : JsonStaticMeshData["TextureData"])
	{
		TextureResourcePath = TextureData["Path"];
		TextureType = TextureData["Type"];
	}

	//이 시점에서 Texture를 만들어서 넘김

	if (EngineAsset == AssetOwner::Engine)
	{
		TextureResourcePath = FPPathManager::Get().GetEngineAssetPath("TextureResource/" + TextureResourcePath);
	}
	else
	{
		TextureResourcePath = FPPathManager::Get().GetAssetPath("TextureResource/" + TextureResourcePath);
	}

	if (TextureType == "WIC")
	{
		RenderingDevice::GetRenderingDevice().CreateWICTextureFromFile(TextureResourcePath, &Data.TextureResource);
	}

	if (TextureType == "DDS")
	{
		RenderingDevice::GetRenderingDevice().CreateDDSTextureFromFile(TextureResourcePath, &Data.TextureResource);
	}

	RenderingDevice::GetRenderingDevice().GetTextureDesc(Data.TextureResource, &Data.TextureDESC);

	AssetManager->AddTextureData(TextureName, Data);
}
