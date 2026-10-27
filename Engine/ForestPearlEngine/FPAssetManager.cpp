#include "FPAssetManager.h"
#include <iostream>
#include "Utility/FPPathManager.h"
#include "FPGameInstance.h"
#include "FPMaterial.h"
#include "FPMaterialInterface.h"
#include "FPStaticMesh.h"
#include "Renderers/RenderingDevice.h"
#include "Shader/ShaderFactory.h"


////////////////////////////
//
// Buffer Maker
//
void* FPAssetManager::MakePositionVertexBuffer(std::vector<struct POSITION_VERTEX>& Mesh)
{
	if (Mesh.size() == 0) return nullptr;
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(POSITION_VERTEX));
}

void* FPAssetManager::MakeColorVertexBuffer(std::vector<struct COLOR_VERTEX>& Mesh)
{
	if (Mesh.size() == 0) return nullptr;
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(COLOR_VERTEX));
}

void* FPAssetManager::MakeNormalVertexBuffer(std::vector<struct NORMAL_VERTEX>& Mesh)
{
	if (Mesh.size() == 0) return nullptr;
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(NORMAL_VERTEX));
}

void* FPAssetManager::MakeUVVertexBuffer(std::vector<struct UV_VERTEX>& Mesh)
{
	if (Mesh.size() == 0) return nullptr;
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(UV_VERTEX));
}

void* FPAssetManager::MakeIndex16BitBuffer(std::vector<struct INDEX_16BIT>& Mesh)
{
	if (Mesh.size() == 0) return nullptr;
	return RenderingDevice::GetRenderingDevice().CreateIndexBuffer(Mesh.data(), Mesh.size(), sizeof(INDEX_16BIT));
}

void* FPAssetManager::MakeIndex32BitBuffer(std::vector<struct INDEX_32BIT>& Mesh)
{
	if (Mesh.size() == 0) return nullptr;
	return RenderingDevice::GetRenderingDevice().CreateIndexBuffer(Mesh.data(), Mesh.size(), sizeof(INDEX_32BIT));
}

//통합 Mesh Buffer 제작
void FPAssetManager::MakeMeshBuffer(FPMeshData& MeshData)
{
	void* PositionVB = MakePositionVertexBuffer(MeshData.Position_Vertices);
	void* ColorVB = MakeColorVertexBuffer(MeshData.Color_Vertices);
	void* NormalVB = MakeNormalVertexBuffer(MeshData.Normal_Vertices);
	void* UVVB = MakeUVVertexBuffer(MeshData.UV_Vertices);
	void* Index32B = MakeIndex32BitBuffer(MeshData.Index);

	//버퍼 생성 후 Map에 정보 등록 
	AddPositionVertexBuffer(MeshData.Name, PositionVB, MeshData.Position_Vertices.size() * sizeof(POSITION_VERTEX), MeshData.Position_Vertices.size(), sizeof(POSITION_VERTEX), 0);
	AddColorVertexBuffer(MeshData.Name, ColorVB, MeshData.Color_Vertices.size() * sizeof(COLOR_VERTEX), MeshData.Color_Vertices.size(), sizeof(COLOR_VERTEX), 0);
	AddNormalVertexBuffer(MeshData.Name, NormalVB, MeshData.Normal_Vertices.size() * sizeof(NORMAL_VERTEX), MeshData.Normal_Vertices.size(), sizeof(NORMAL_VERTEX), 0);
	AddUVVertexBuffer(MeshData.Name, UVVB, MeshData.UV_Vertices.size() * sizeof(UV_VERTEX), MeshData.UV_Vertices.size(), sizeof(UV_VERTEX), 0);
	AddIndexBuffer(MeshData.Name, Index32B, MeshData.Index.size() * sizeof(INDEX_32BIT), MeshData.Index.size() * 3, MeshData.Index.size(), sizeof(INDEX_32BIT), 0);
}

//Vertex Shader 제작
void FPAssetManager::MakeVertexShader(FPVertexShaderData& VertexShaderData)
{
	std::wstring FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetAssetShaderPath(VertexShaderData.Path)));
	if (VertexShaderData.Owner == AssetOwner::Engine) FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetEngineAssetShaderPath(VertexShaderData.Path)));

	void* VertexShader = nullptr;
	void* VSCode = nullptr;
	void* VBLayout = nullptr;

	if (VertexShaderData.InputLayout == "Default")
	{
		ShaderFactory::GetShaderFactory().VertexShaderLoad(FilePath.c_str(), &VertexShader, &VSCode, &VBLayout);
	}

	if (VertexShaderData.InputLayout == "NormalDebug")
	{
		ShaderFactory::GetShaderFactory().NormalVertexShaderLoad(FilePath.c_str(), &VertexShader, &VSCode, &VBLayout);
	}

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	AssetManager->AddVertexShader(VertexShaderData.Name, VertexShader, VSCode, VBLayout);
}

//Pixel Shader 제작
void FPAssetManager::MakePixelShader(FPPixelShaderData& PixelShaderData)
{
	std::wstring FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetAssetShaderPath(PixelShaderData.Path)));
	if (PixelShaderData.Owner == AssetOwner::Engine) FilePath = FPPathManager::Get().StringToWString((FPPathManager::Get().GetEngineAssetShaderPath(PixelShaderData.Path)));

	void* PixelShader = nullptr;
	void* PSCode = nullptr;

	ShaderFactory::GetShaderFactory().PixelShaderLoad(FilePath.c_str(), &PixelShader, &PSCode);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	AssetManager->AddPixelShader(PixelShaderData.Name, PixelShader, PSCode);
}

//모든 Asset Data 객체를 생성
void FPAssetManager::AllAssetDataCreate()
{
	//Texture 생성
	for (auto& [AssetName, Asset] : LoadedTextureData)
	{
		MakeTexture(Asset);
	}

	//Fbx Load
	for (auto& [AssetName, Asset] : LoadedStaticMeshData)
	{
		LoadFbxData(Asset);
	}

	//Mesh Buffer 생성
	for (auto& [AssetName, Asset] : LoadedMeshData)
	{
		MakeMeshBuffer(Asset);
	}

	//Vertex Buffer 생성
	for (auto& [AssetName, Asset] : LoadedVertexShaderData)
	{
		MakeVertexShader(Asset);
	}

	//Pixel Buffer 생성
	for (auto& [AssetName, Asset] : LoadedPixelShaderData)
	{
		MakePixelShader(Asset);
	}
}

////////////////////////////
//
// Gizmo Mesh Maker
//
void FPAssetManager::MakeGizmoVertexBuffer(std::vector<struct VERTEX>& Mesh, const std::string& GizemoMeshPath)
{
	if (HasStaticMeshData(GizemoMeshPath))
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
	AddMeshData(GizemoMeshPath, MeshData);
	MakeMeshBuffer(MeshData);
}

////////////////////////////
//
// Mesh
//
void FPAssetManager::AddMeshData(std::string ModelNodeName, FPMeshData& MeshData)
{
	LoadedMeshData[ModelNodeName] = MeshData;
}

FPMeshData& FPAssetManager::GetMeshData(std::string FbxPath)
{
	return LoadedMeshData[FbxPath];
}

bool FPAssetManager::HasMeshData(std::string ModelNodeName)
{
	if (LoadedMeshData.count(ModelNodeName) <= 0)
	{
		return false;
	}
	return true;
}

void FPAssetManager::AddPositionVertexBuffer(std::string FbxPath, FPVertexBufferData& PositionVertexBuffer)
{
	MeshPositionVertexBuffer[FbxPath] = PositionVertexBuffer;
}

void FPAssetManager::AddPositionVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.VB_Size = VBSize;
	VBData.VB_Cnt = VBCnt;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshPositionVertexBuffer[FbxPath] = VBData;
}

bool FPAssetManager::HasPositionBuffer(std::string MeshPath)
{
	if (MeshPositionVertexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

FPVertexBufferData FPAssetManager::GetPositionVertexBuffer(std::string MeshPath)
{
	if (!HasPositionBuffer(MeshPath))
	{
		std::cout << MeshPath << "의 VB Position 데이터가 없습니다." << "\n";

		return {};
	}

	return MeshPositionVertexBuffer[MeshPath];
}

void FPAssetManager::AddColorVertexBuffer(std::string FbxPath, FPVertexBufferData& ColorVertexBuffer)
{
	MeshColorVertexBuffer[FbxPath] = ColorVertexBuffer;
}

void FPAssetManager::AddColorVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.VB_Size = VBSize;
	VBData.VB_Cnt = VBCnt;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshColorVertexBuffer[FbxPath] = VBData;
}

bool FPAssetManager::HasColorBuffer(std::string MeshPath)
{
	if (MeshColorVertexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

FPVertexBufferData FPAssetManager::GetColorVertexBuffer(std::string MeshPath)
{
	if (!HasColorBuffer(MeshPath))
	{
		std::cout << MeshPath << "의 VB Color 데이터가 없습니다." << "\n";

		return {};
	}

	return MeshColorVertexBuffer[MeshPath];
}

void FPAssetManager::AddNormalVertexBuffer(std::string FbxPath, FPVertexBufferData& NormalVertexBuffer)
{
	MeshNormalVertexBuffer[FbxPath] = NormalVertexBuffer;
}

void FPAssetManager::AddNormalVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.VB_Size = VBSize;
	VBData.VB_Cnt = VBCnt;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshNormalVertexBuffer[FbxPath] = VBData;
}

bool FPAssetManager::HasNormalBuffer(std::string MeshPath)
{
	if (MeshNormalVertexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

FPVertexBufferData FPAssetManager::GetNormalVertexBuffer(std::string MeshPath)
{
	if (!HasNormalBuffer(MeshPath))
	{
		std::cout << MeshPath << "의 VB Normal 데이터가 없습니다." << "\n";

		return {};
	}

	return MeshNormalVertexBuffer[MeshPath];
}

void FPAssetManager::AddUVVertexBuffer(std::string FbxPath, FPVertexBufferData& UVVertexBuffer)
{
	MeshUVVertexBuffer[FbxPath] = UVVertexBuffer;
}

void FPAssetManager::AddUVVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.VB_Size = VBSize;
	VBData.VB_Cnt = VBCnt;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshUVVertexBuffer[FbxPath] = VBData;
}

bool FPAssetManager::HasUVBuffer(std::string MeshPath)
{
	if (MeshUVVertexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

FPVertexBufferData FPAssetManager::GetUVVertexBuffer(std::string MeshPath)
{
	if (!HasUVBuffer(MeshPath))
	{
		std::cout << MeshPath << "의 VB UV 데이터가 없습니다." << "\n";

		return {};
	}

	return MeshUVVertexBuffer[MeshPath];
}

void FPAssetManager::AddIndexBuffer(std::string FbxPath, FPIndexBufferData& PositionVertexBuffer)
{
	MeshIndexBuffer[FbxPath] = PositionVertexBuffer;
}

void FPAssetManager::AddIndexBuffer(std::string FbxPath, void* IndexBuffer, unsigned long IBSize, unsigned long IBCnt, unsigned long FaceCnt, unsigned long Stride, unsigned long Offset)
{
	FPIndexBufferData IBData;
	IBData.IndexBuffer = IndexBuffer;
	IBData.Index_Size = IBSize;
	IBData.Index_Cnt = IBCnt;
	IBData.FaceCnt = FaceCnt;
	IBData.Stride = Stride;
	IBData.Offset = Offset;

	MeshIndexBuffer[FbxPath] = IBData;
}

bool FPAssetManager::HasIndexBuffer(std::string MeshPath)
{
	if (MeshIndexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

FPIndexBufferData FPAssetManager::GetIndexBuffer(std::string MeshPath)
{
	if (!HasIndexBuffer(MeshPath))
	{
		std::cout << MeshPath << "의 IB 데이터가 없습니다." << "\n";

		return {};
	}

	return MeshIndexBuffer[MeshPath];
}

////////////////////////////
//
// FBX Load
//
void FPAssetManager::LoadFbxData(FPStaticMeshData& StaticMeshData)
{
	//fbx Load 옵션 설정
	ufbx_load_opts Opts = {};
	Opts.target_axes = ufbx_axes_left_handed_y_up;
	Opts.target_unit_meters = 1.0f;

	ufbx_error Error;

	std::string FilePath = (FPPathManager::Get().GetAssetPath("Model/" + StaticMeshData.MeshPath));
	if (StaticMeshData.Owner == AssetOwner::Engine) FilePath = (FPPathManager::Get().GetEngineAssetPath("Model/" + StaticMeshData.MeshPath));

	ufbx_scene* Scene = ufbx_load_file(FilePath.c_str(), &Opts, &Error);
	if (!Scene)
	{
		fprintf(stderr, "Failed to load Scene : %s\n", Error.description.data);
	}

	for (ufbx_node* Node : Scene->nodes)
	{
		if (!Node) { continue; }

		const std::string NodeName = ConvertUfbxString(Node->name);
		//std::cout << NodeName << "\n";

		if (NodeName != StaticMeshData.NodeName) { continue; }

		// 본, 카메라, 라이트, 빈 노드 등 메시가 없는 요소는 건너 뜀
		if (!Node->mesh) { continue; }

		const ufbx_mesh* Mesh = Node->mesh;

		//메시 정점 추출
		FPMeshData MeshData = ConvertUfbxMesh(Mesh, Node);
		MeshData.Name = StaticMeshData.Name;


		if (MeshData.NodeName.empty())
		{
			MeshData.NodeName = NodeName;
		}

		AddMeshData(StaticMeshData.Name, MeshData);
	}
	ufbx_free_scene(Scene);
}

//ufbx_string -> std::string
std::string FPAssetManager::ConvertUfbxString(ufbx_string String)
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
FPMeshData FPAssetManager::ConvertUfbxMesh(const ufbx_mesh* Mesh, const ufbx_node* Node)
{
	FPMeshData Result;

	if (!Mesh)
	{
		return Result;
	}

	Result.NodeName = ConvertUfbxString(Node->name);

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
			UVVertex.v = 1.0f - static_cast<float>(UV.y);

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

////////////////////////////
//
// StaticMesh
//
FPStaticMesh* FPAssetManager::GetStaticMeshData(std::string StaticMeshName)
{
	FPStaticMeshData StaticMeshData = LoadedStaticMeshData[StaticMeshName];
	FPStaticMesh* StaticMesh = new FPStaticMesh(StaticMeshData.Name);

	for (SOCKET_TRANSFORM& Socket : StaticMeshData.Sockets)
	{
		FTransform SocketTransform;
		SocketTransform.Location.x = Socket.Location_x;
		SocketTransform.Location.y = Socket.Location_y;
		SocketTransform.Location.z = Socket.Location_z;

		SocketTransform.Rotation.x = Socket.Rotation_x;
		SocketTransform.Rotation.y = Socket.Rotation_y;
		SocketTransform.Rotation.z = Socket.Rotation_z;

		SocketTransform.Scale.x = Socket.Scale_x;
		SocketTransform.Scale.y = Socket.Scale_y;
		SocketTransform.Scale.z = Socket.Scale_Z;
		
		StaticMesh->AddSocketData(Socket.SocketName, SocketTransform);
	}

	FPMaterialInterface* Material = MakeMaterial(StaticMeshData.MaterialName);
	StaticMesh->SetMaterial(Material);

	return StaticMesh;
}

FPStaticMeshData& FPAssetManager::GetStaticMeshData_(std::string StaticMeshName)
{
	return LoadedStaticMeshData[StaticMeshName];
}

void FPAssetManager::AddStaticMeshData(std::string StaticMeshName, FPStaticMeshData* StaticMeshData)
{
	LoadedStaticMeshData[StaticMeshName] = std::move(*StaticMeshData);
}

bool FPAssetManager::HasStaticMeshData(std::string StaticMeshName)
{
	if (LoadedStaticMeshData.count(StaticMeshName) > 0)
	{
		return true;
	}

	return false;
}

////////////////////////////
//
// Texture
//

FPTextureData& FPAssetManager::GetTextureData(std::string TextureName)
{
	return this->TextureData[TextureName];
}

void FPAssetManager::AddTextureData(std::string TextureName, FPTextureData& TextureData)
{
	this->TextureData[TextureName] = TextureData;
}

bool FPAssetManager::HasTextureData(std::string TextureName)
{
	if (TextureData.count(TextureName) > 0)
	{
		return true;
	}

	return false;
}

void FPAssetManager::MakeTexture(FPTextureInfo& TextureInfo)
{
	//이 시점에서 Texture를 만들어서 넘김
	FPTextureData Data;

	std::string TextureResourcePath = TextureInfo.Path;

	if (TextureInfo.Owner == AssetOwner::Engine)
	{
		TextureResourcePath = FPPathManager::Get().GetEngineAssetPath("TextureResource/" + TextureResourcePath);
	}
	else
	{
		TextureResourcePath = FPPathManager::Get().GetAssetPath("TextureResource/" + TextureResourcePath);
	}

	if (TextureInfo.Type == "WIC")
	{
		RenderingDevice::GetRenderingDevice().CreateWICTextureFromFile(TextureResourcePath, &Data.TextureResource);
	}

	if (TextureInfo.Type == "DDS")
	{
		RenderingDevice::GetRenderingDevice().CreateDDSTextureFromFile(TextureResourcePath, &Data.TextureResource);
	}

	RenderingDevice::GetRenderingDevice().GetTextureDesc(Data.TextureResource, &Data.TextureDESC);

	AddTextureData(TextureInfo.Name, Data);
}

////////////////////////////
//
// Texture Info
//
FPTextureInfo& FPAssetManager::GetTextureInfoData(std::string TextureName)
{
	return this->LoadedTextureData[TextureName];
}

void FPAssetManager::AddTextureInfoData(std::string TextureName, FPTextureInfo& TextureData)
{
	this->LoadedTextureData[TextureName] = TextureData;
}

bool FPAssetManager::HasTextureInfoData(std::string TextureName)
{
	if (LoadedTextureData.count(TextureName) > 0)
	{
		return true;
	}

	return false;
}

////////////////////////////
//
// Texture Array Info
//
FPTextureArrayInfo& FPAssetManager::GetTextureArrayInfoData(std::string TextureArrayName)
{
	return this->LoadedTextureArrayData[TextureArrayName];
}

void FPAssetManager::AddTextureArrayInfoData(std::string TextureArrayName, FPTextureArrayInfo& TextureArrayData)
{
	this->LoadedTextureArrayData[TextureArrayName] = TextureArrayData;
}

bool FPAssetManager::HasTextureArrayInfoData(std::string TextureArrayName)
{
	if (LoadedTextureArrayData.count(TextureArrayName) > 0)
	{
		return true;
	}

	return false;
}

FPMaterialData& FPAssetManager::GetMaterialData(std::string MaterialName)
{
	return this->LoadedMaterialData[MaterialName];
}

void FPAssetManager::AddMaterialData(std::string MaterialName, FPMaterialData& MaterialData)
{
	this->LoadedMaterialData[MaterialName] = MaterialData;
}

bool FPAssetManager::HasMaterialData(std::string MaterialName)
{
	if (LoadedMaterialData.count(MaterialName) > 0)
	{
		return true;
	}

	return false;
}

bool FPAssetManager::HasGameSettingData()
{
	if (GameSettingData != nullptr)
	{
		return true;
	}

	return false;
}

void FPAssetManager::UpdateGameSettingData(FPGameSettingData& GameSettingData)
{
	if (!HasGameSettingData())
	{
		this->GameSettingData = new FPGameSettingData();
	}

	*(this->GameSettingData) = GameSettingData;
}

FPGameSettingData* FPAssetManager::GetGameSettingData()
{
	return GameSettingData;
}


////////////////////////////
//
// Level
//
bool FPAssetManager::HasLevelData(std::string LevelName)
{
	if (LevelData.count(LevelName) > 0)
	{
		return true;
	}

	return false;
}

void FPAssetManager::AddLevelData(std::string LevelName, FPActorData* ActorData)
{
	LevelData[LevelName].push_back(std::move(*ActorData));
}


std::vector<FPActorData>& FPAssetManager::GetLevelData(std::string LevelName)
{
	return LevelData[LevelName];
}

////////////////////////////
//
// GameMode
//
void FPAssetManager::AddGameModeData(std::string LevelName, std::string GameModeName)
{
	GameModeData[LevelName] = GameModeName;
}

std::string FPAssetManager::GetGameModeData(std::string LevelName)
{
	return GameModeData[LevelName];
}

////////////////////////////
//
// Material
//
FPMaterialInterface* FPAssetManager::MakeMaterial(std::string MaterialName)
{
	std::string ResolvedMaterialName = HasMaterialData(MaterialName) ? MaterialName : "DefaultMaterial";
	FPMaterialData& MaterialData = GetMaterialData(ResolvedMaterialName);

	FPMaterial* Material = new FPMaterial();
	Material->SetVertexShader(MaterialData.VertexShader);
	Material->SetPixelShader(MaterialData.PixelShader);

	FPVector4 BaseColor{ MaterialData.BaseColor.r, MaterialData.BaseColor.g, MaterialData.BaseColor.b, MaterialData.BaseColor.a };
	FPVector4 Diffuse{ MaterialData.Diffuse.r, MaterialData.Diffuse.g, MaterialData.Diffuse.b, MaterialData.Diffuse.a };
	FPVector4 Ambient{ MaterialData.Ambient.r, MaterialData.Ambient.g, MaterialData.Ambient.b, MaterialData.Ambient.a };
	FPVector4 Emissive{ MaterialData.Emissive.r, MaterialData.Emissive.g, MaterialData.Emissive.b, MaterialData.Emissive.a };

	Material->SetBaseColor(BaseColor);
	Material->SetDiffuse(Diffuse);
	Material->SetAmbient(Ambient);
	Material->SetEmissive(Emissive);
	Material->SetTwoSided(MaterialData.TwoSided);

	for (auto& [Slot, TextureName, TextureType, SamplerType] : MaterialData.Textures)
	{
		Material->AddTexture(Slot, TextureName, SamplerType, TextureType);
	}

	return Material;
}

FPVertexShaderData& FPAssetManager::GetVertexShaderData(std::string VertexShaderName)
{
	return this->LoadedVertexShaderData[VertexShaderName];
}

void FPAssetManager::AddVertexShaderData(std::string VertexShaderName, FPVertexShaderData& VertexShaderData)
{
	this->LoadedVertexShaderData[VertexShaderName] = VertexShaderData;
}

bool FPAssetManager::HasVertexShaderData(std::string VertexShaderName)
{
	if (LoadedVertexShaderData.count(VertexShaderName) > 0)
	{
		return true;
	}

	return false;
}

FPPixelShaderData& FPAssetManager::GetPixelShaderData(std::string PixelShaderName)
{
	return this->LoadedPixelShaderData[PixelShaderName];
}

void FPAssetManager::AddPixelShaderData(std::string PixelShaderName, FPPixelShaderData& PixelShaderData)
{
	this->LoadedPixelShaderData[PixelShaderName] = PixelShaderData;
}

bool FPAssetManager::HasPixelShaderData(std::string PixelShaderName)
{
	if (LoadedPixelShaderData.count(PixelShaderName) > 0)
	{
		return true;
	}

	return false;
}

////////////////////////////
//
// Shader
//
bool FPAssetManager::HasVertexShader(std::string ShaderName)
{
	if (VertexShaderData.count(ShaderName) > 0)
	{
		return true;
	}
	return false;
}

bool FPAssetManager::HasPixelShader(std::string ShaderName)
{
	if (PixelShaderData.count(ShaderName) > 0)
	{
		return true;
	}
	return false;
}

void FPAssetManager::AddVertexShader(std::string ShaderName, void* VertexShader, void* VSCode, void* VBLayout)
{
	VertexShaderData[ShaderName] = { VertexShader, VSCode, VBLayout };
}

void FPAssetManager::AddPixelShader(std::string ShaderName, void* PixelShader, void* PSCode)
{
	PixelShaderData[ShaderName] = { PixelShader, PSCode };
}

std::tuple<void*, void*, void*> FPAssetManager::GetVertexShader(std::string ShaderName)
{
	if (VertexShaderData.count(ShaderName) <= 0)
	{
		std::cout << ShaderName << "의 Vertex Shader 데이터가 없습니다." << "\n";

		return {};
	}
	return VertexShaderData[ShaderName];
}

std::pair<void*, void*> FPAssetManager::GetPixelShader(std::string ShaderName)
{
	if (PixelShaderData.count(ShaderName) <= 0)
	{
		std::cout << ShaderName << "의 Pixel Shader 데이터가 없습니다." << "\n";

		return {};
	}
	return PixelShaderData[ShaderName];
}











