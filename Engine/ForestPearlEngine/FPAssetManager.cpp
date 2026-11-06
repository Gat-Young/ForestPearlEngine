#include "FPAssetManager.h"
#include <iostream>
#include <set>
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

void* FPAssetManager::MakeTangentVertexBuffer(std::vector<struct TANGENT_VERTEX>& Mesh)
{
	if (Mesh.size() == 0) return nullptr;
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(Mesh.data(), Mesh.size(), sizeof(TANGENT_VERTEX));
}

//통합 Mesh Buffer 제작
void FPAssetManager::MakeMeshBuffer(FPMeshData& MeshData)
{
	void* PositionVB = MakePositionVertexBuffer(MeshData.Position_Vertices);
	void* ColorVB = MakeColorVertexBuffer(MeshData.Color_Vertices);
	void* NormalVB = MakeNormalVertexBuffer(MeshData.Normal_Vertices);
	void* UVVB = MakeUVVertexBuffer(MeshData.UV_Vertices);
	void* Index32B = MakeIndex32BitBuffer(MeshData.Index);
	void* TangentVB = MakeTangentVertexBuffer(MeshData.Tangent_Vertices);

	//버퍼 생성 후 Map에 정보 등록 
	AddPositionVertexBuffer(MeshData.Name, PositionVB, MeshData.Position_Vertices.size() * sizeof(POSITION_VERTEX), MeshData.Position_Vertices.size(), sizeof(POSITION_VERTEX), 0);
	AddColorVertexBuffer(MeshData.Name, ColorVB, MeshData.Color_Vertices.size() * sizeof(COLOR_VERTEX), MeshData.Color_Vertices.size(), sizeof(COLOR_VERTEX), 0);
	AddNormalVertexBuffer(MeshData.Name, NormalVB, MeshData.Normal_Vertices.size() * sizeof(NORMAL_VERTEX), MeshData.Normal_Vertices.size(), sizeof(NORMAL_VERTEX), 0);
	AddUVVertexBuffer(MeshData.Name, UVVB, MeshData.UV_Vertices.size() * sizeof(UV_VERTEX), MeshData.UV_Vertices.size(), sizeof(UV_VERTEX), 0);
	AddIndexBuffer(MeshData.Name, Index32B, MeshData.Index.size() * sizeof(INDEX_32BIT), MeshData.Index.size() * 3, MeshData.Index.size(), sizeof(INDEX_32BIT), 0);
	AddTangentVertexBuffer(MeshData.Name, TangentVB, MeshData.Tangent_Vertices.size() * sizeof(TANGENT_VERTEX), MeshData.Tangent_Vertices.size(), sizeof(TANGENT_VERTEX), 0);
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

	//Texture Array 생성
	for (auto& [AssetName, Asset] : LoadedTextureArrayData)
	{
		MakeTextureArray(Asset);
	}

	//RenderTarget 생성
	for (auto& [AssetName, Asset] : LoadedRenderTargetTexture2DData)
	{
		MakeRenderTarget(Asset);
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

void FPAssetManager::AddTangentVertexBuffer(std::string FbxPath, FPVertexBufferData& TangentVertexBuffer)
{
	MeshTangentVertexBuffer[FbxPath] = TangentVertexBuffer;
}

void FPAssetManager::AddTangentVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.VB_Size = VBSize;
	VBData.VB_Cnt = VBCnt;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshTangentVertexBuffer[FbxPath] = VBData;
}

bool FPAssetManager::HasTangentBuffer(std::string MeshPath)
{
	if (MeshTangentVertexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

FPVertexBufferData FPAssetManager::GetTangentVertexBuffer(std::string MeshPath)
{
	if (!HasTangentBuffer(MeshPath))
	{
		std::cout << MeshPath << "의 Tangent VB 데이터가 없습니다." << "\n";

		return {};
	}

	return MeshTangentVertexBuffer[MeshPath];
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

bool FPAssetManager::CalculateTriangleTangent(const POSITION_VERTEX& P0, const POSITION_VERTEX& P1, const POSITION_VERTEX& P2, const UV_VERTEX& UV0, const UV_VERTEX& UV1, const UV_VERTEX& UV2, FPVector3& OutTangent, FPVector3& OutBinormal)
{
	FPVector4 V0{ P0.x, P0.y, P0.z, 0.0f };
	FPVector4 V1{ P1.x, P1.y, P1.z, 0.0f };
	FPVector4 V2{ P2.x, P2.y, P2.z, 0.0f };

	FPVector4 E1 = V1 - V0;
	FPVector4 E2 = V2 - V0;

	float DeltaU1 = UV1.u - UV0.u;
	float DeltaV1 = UV1.v - UV0.v;

	float DeltaU2 = UV2.u - UV0.u;
	float DeltaV2 = UV2.v - UV0.v;

	float Deter = DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1;

	//퇴화한 삼각형 검사
	float UVScale = (std::max)((std::max)(std::fabs(DeltaU1), std::fabs(DeltaV1)),(std::max)(std::fabs(DeltaU2), std::fabs(DeltaV2)));
	
	if (UVScale < 1e-12f || std::fabs(Deter) < 1e-6f * UVScale * UVScale)
	{
		return false;
	}

	// Position 삼각형 퇴화 검사
	FPVector3 Edge1{ E1.x, E1.y, E1.z };
	FPVector3 Edge2{ E2.x, E2.y, E2.z };

	FPVector3 CrossVec = Cross(Edge1, Edge2);

	float Edge1Sq = Dot(Edge1, Edge1);
	float Edge2Sq = Dot(Edge2, Edge2);

	if (Edge1Sq < 1e-20f ||
		Edge2Sq < 1e-20f ||
		Dot(CrossVec, CrossVec) < 1e-12f * Edge1Sq * Edge2Sq)
	{
		return false;
	}

	float InvD = 1.0f / Deter;

	FPVector4 T;
	
	T = ((E1 * DeltaV2) - (E2 * DeltaV1)) * InvD;

	OutTangent = {T.x , T.y , T.z};

	FPVector4 B;

	B = ((E1 * -DeltaU2) + (E2 * DeltaU1)) * InvD;

	OutBinormal = { B.x, B.y, B.z };

	return true;
}

//같은 원본 정점 그룹 내에서 Normal과 UV가 같은지 확인
bool FPAssetManager::SameTangentAttributes(const FPMeshData& Mesh, uint32_t A, uint32_t B)
{
	const NORMAL_VERTEX& N0 = Mesh.Normal_Vertices[A];
	const NORMAL_VERTEX& N1 = Mesh.Normal_Vertices[B];

	const UV_VERTEX& U0 = Mesh.UV_Vertices[A];
	const UV_VERTEX& U1 = Mesh.UV_Vertices[B];

	return  N0.nx == N1.nx &&
			N0.ny == N1.ny &&
			N0.nz == N1.nz &&
			U0.u == U1.u &&
			U0.v == U1.v;
}

//Tanget 누적시 자연스럽게 만들기 위한 삼각형 코너 각도 가중치 계산
float FPAssetManager::CalculateCornerAngle(const FPMeshData& Mesh, size_t Base, size_t LocalCorner)
{
	const POSITION_VERTEX& P_Vertex = Mesh.Position_Vertices[Base + LocalCorner];
	const FPVector3 P{ P_Vertex.x , P_Vertex.y , P_Vertex.z };

	const POSITION_VERTEX& A_Vertex = Mesh.Position_Vertices[Base + (LocalCorner+1)%3];
	const FPVector3 A{ A_Vertex.x , A_Vertex.y , A_Vertex.z };

	const POSITION_VERTEX& B_Vertex = Mesh.Position_Vertices[Base + (LocalCorner + 2) % 3];
	const FPVector3 B{ B_Vertex.x, B_Vertex.y, B_Vertex.z };

	FPVector3 E1 = A - P;
	FPVector3 E2 = B - P;

	FPVector3 C = Cross(E1, E2);

	return std::atan2(std::sqrt(Dot(C, C)), Dot(E1, E2));
}

bool FPAssetManager::GenerateTangents(FPMeshData& MeshData)
{
	struct FPCornerTangentData
	{
		FPVector3 Tangent{};

		float TangentSign = 1.0f;
		float Weight = 0.0f;

		bool Valid = false;
	};

	const size_t VertexCount = MeshData.Position_Vertices.size();

	// 1. 입력 데이터 검증
	if (VertexCount == 0 || VertexCount % 3 != 0) { return false; }
	if (MeshData.Normal_Vertices.size() != VertexCount || MeshData.UV_Vertices.size() != VertexCount) { return false; }
	
	//최종 Tanget 저장 공간 초기화
	MeshData.Tangent_Vertices.assign(VertexCount, TANGENT_VERTEX{});

	//삼각형 코너별 임시 데이터
	std::vector<FPCornerTangentData> CornerData(VertexCount);

	//최종 Tangent 할당 여부
	std::vector<uint8_t> Processed(VertexCount, 0);

	//모든 삼각형의 Tangent / Binormal 계산

	for (size_t i = 0; i < VertexCount; i += 3)
	{
		FPVector3 Traw{};
		FPVector3 Braw{};

		bool Success = CalculateTriangleTangent(MeshData.Position_Vertices[i],
												MeshData.Position_Vertices[i+1],
												MeshData.Position_Vertices[i+2],
			
												MeshData.UV_Vertices[i],
												MeshData.UV_Vertices[i+1],
												MeshData.UV_Vertices[i+2],
			
												Traw,
												Braw);

		if (!Success) { continue; } //퇴화 삼각형은 누적 기여 X

		//한 삼각형에 세 코너 처리
		for (size_t j = 0; j < 3; ++j)
		{
			const size_t Corner = i + j;

			const NORMAL_VERTEX& N_Vertex = MeshData.Normal_Vertices[Corner];
			FPVector3 N{ N_Vertex.nx, N_Vertex.ny, N_Vertex.nz };

			N = N.Normalize();

			//Gram - Schmidt 직교
			//Tangent를 Normal에 직교
			FPVector3 T = Traw - (N * Dot(N, Traw));

			T = T.Normalize();

			//TangentSign 계산
			float TangentSign = Dot(Cross(N, T), Braw);
			if (!std::isfinite(TangentSign) || std::fabs(TangentSign) < 1e-20f)
			{
				continue;
			}

			TangentSign = TangentSign < 0.0f ? -1.0f : 1.0f;

			//삼각형 코너 각도
			float Weight = CalculateCornerAngle(MeshData, i, j);

			if (!std::isfinite(Weight) || Weight <= 0.0f)
			{
				continue;
			}

			//코너별 임시 Tanget에 저장
			CornerData[Corner].Tangent = T;
			CornerData[Corner].TangentSign = TangentSign;
			CornerData[Corner].Weight = Weight;
			CornerData[Corner].Valid = true;
		}
	}

	//3. 원본 정점 인접 정보를 이용한 Tanget 누적
	for (size_t v = 0; v < MeshData.VertexAdjacency.size(); ++v)
	{
		const std::vector<uint32_t>& AdjacentCorners = MeshData.VertexAdjacency[v];

		for (size_t a = 0; a < AdjacentCorners.size(); ++a)
		{
			const uint32_t Seed = AdjacentCorners[a];
			
			if (Seed >= VertexCount) { return false; }
			if (Processed[Seed]) { continue; }

			//그룹별 Tangent Sign 결정
			float GroupSign = 1.0f;
			bool HasSign = false;

			//Seed와 속성이 같은 유효 코너에서 부호 검색
			for (size_t b = 0; b < AdjacentCorners.size(); ++b)
			{
				const uint32_t Candidate = AdjacentCorners[b];
				if (Candidate >= VertexCount) { return false; }
				if (Processed[Candidate] || !CornerData[Candidate].Valid) { continue; }

				if (!SameTangentAttributes(MeshData, Seed, Candidate)) { continue; }

				GroupSign = CornerData[Candidate].TangentSign;
				HasSign = true;
				break;
			}

			//같은 그룹의 Tanget 누적
			std::vector<uint32_t> GroupCorners;
			FPVector3 TangentSum{};
			FPVector3 FirstValidTangent{};
			bool HasValidTangent = false;

			for (size_t b = 0; b < AdjacentCorners.size(); ++b)
			{
				const uint32_t Candidate = AdjacentCorners[b];
				if (Processed[Candidate]) { continue; }

				//Normal / UV가 다르면 분리
				if (!SameTangentAttributes(MeshData, Seed, Candidate)) { continue; }

				//미리 Tangent Sign이 다르면 분리
				if (HasSign && CornerData[Candidate].Valid && CornerData[Candidate].TangentSign != GroupSign) { continue; }

				GroupCorners.push_back(Candidate);

				if (CornerData[Candidate].Valid)
				{
					const FPVector3& T = CornerData[Candidate].Tangent;
					const float Weight = CornerData[Candidate].Weight;

					TangentSum = TangentSum + (T * Weight);

					if (!HasValidTangent)
					{
						FirstValidTangent = T;
						HasValidTangent = true;
					}
				}
			}

			if (GroupCorners.empty())
			{
				return false;
			}

			//4. 그룹별 최종 Tanget 생성
			const NORMAL_VERTEX& N_Vertex = MeshData.Normal_Vertices[Seed];
			FPVector3 N{ N_Vertex.nx , N_Vertex.ny, N_Vertex.nz };

			N = N.Normalize();

			//누적 Tanget를 Notmal에 직교
			FPVector3 T = TangentSum - (N * Dot(N, TangentSum));

			T = T.Normalize();

			//유효하지 않은 경우
			if (T.x == 0 && T.y == 0 && T.z == 0)
			{
				if (HasValidTangent)
				{
					//유효했던 첫 번째 Tangent 사용
					T = FirstValidTangent - (N * Dot(N, FirstValidTangent));
					T = T.Normalize();
				}

				if (!HasValidTangent || ((T.x == 0 && T.y == 0) && T.z == 0))
				{
					//모든 Tanget가 유효하지 않은 경우
					//Normal 기준 대체 Tangent 생성
					FPVector3 Axis;

					if (std::fabs(N.x) < 0.9f)
					{
						Axis = { 1.0f, 0.0f, 0.0f };
					}
					else
					{
						Axis = { 0.0f, 1.0f, 0.0f };
					}

					T = Cross(Axis, N);
					T = T.Normalize();
				}
			}


			//5. 같은 그룹의 모든 코너에 Tanget 기록

			for (size_t g = 0; g < GroupCorners.size(); ++g)
			{
				const uint32_t Index = GroupCorners[g];

				TANGENT_VERTEX& Output = MeshData.Tangent_Vertices[Index];

				Output.x = T.x;
				Output.y = T.y;
				Output.z = T.z;
				Output.w = GroupSign;

				Processed[Index] = 1;
			}
		}
	}

	//6. 모든 코너가 처리되었는지 검사
	for (size_t i = 0; i < VertexCount; ++i)
	{
		if (!Processed[i])
		{
			return false;
		}
	}

	return true;
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
FPMeshData FPAssetManager::ConvertUfbxMesh(const ufbx_mesh* Mesh, const ufbx_node* Node)
{
	FPMeshData Result;

	if (!Mesh || !Node)
	{
		return Result;
	}
	// 원본 정점 하나를 참조하는
	// 삼각형 코너 인덱스들의 목록
	Result.VertexAdjacency.resize(Mesh->num_vertices);

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

			// ufbx 원본 논리적 정점 인덱스
			const uint32_t OriginalVertexIndex = Mesh->vertex_indices[CornerIndex];

			// 엔진의 삼각형 코너 배열 인덱스
			const uint32_t ExpandedCornerIndex = static_cast<uint32_t>(Result.Position_Vertices.size());

			// 원본 정점에 연결된 코너 저장
			Result.VertexAdjacency[OriginalVertexIndex].push_back(ExpandedCornerIndex);


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
	
	//Tangent Space 생성
	const size_t TotalVertexCount = Result.Position_Vertices.size();
	if (TotalVertexCount == 0) { return Result; }

	bool HasTangent = false;
	if (Mesh->vertex_normal.exists && Mesh->vertex_uv.exists)
	{
		HasTangent = GenerateTangents(Result);
		if (!HasTangent)
		{
			return FPMeshData{};
		}
	}

	// Index Buffer 생성
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
		},
		{
			Result.Tangent_Vertices.data(),
			TotalVertexCount,
			sizeof(TANGENT_VERTEX)
		}
	};

	// 임시 Index 배열
	std::vector<uint32_t> Indices(TotalVertexCount);

	ufbx_error Error{};

	// 중복 정점 제거
	const size_t UniqueVertexCount = ufbx_generate_indices(
		VertexStreams,
		5,				
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
		Result.VertexAdjacency.clear();

		return Result;
	}

	// 기존의 원본 정점 기반 인접 정보 초기화
	Result.VertexAdjacency.clear();

	// 최종 Vertex Buffer 정점 수로 재설정
	Result.VertexAdjacency.resize(UniqueVertexCount);

	// 최종 인덱스를 이용한 인접 삼각형 구축
	for (size_t i = 0; i < Indices.size(); i += 3)
	{
		const uint32_t TriangleIndex =
			static_cast<uint32_t>(i / 3);

		const uint32_t A = Indices[i];
		const uint32_t B = Indices[i + 1];
		const uint32_t C = Indices[i + 2];

		Result.VertexAdjacency[A].push_back(TriangleIndex);
		// 퇴화 삼각형으로 인한 중복 등록 방지
		if (B != A)
		{
			Result.VertexAdjacency[B].push_back(TriangleIndex);
		}

		if (C != A && C != B)
		{
			Result.VertexAdjacency[C].push_back(TriangleIndex);
		}
	}

	// --------------------------------------------------------
	// 3. 고유 정점 개수만큼 축소
	// --------------------------------------------------------

	Result.Position_Vertices.resize(UniqueVertexCount);
	Result.Normal_Vertices.resize(UniqueVertexCount);
	Result.Color_Vertices.resize(UniqueVertexCount);
	Result.UV_Vertices.resize(UniqueVertexCount);
	Result.Tangent_Vertices.resize(UniqueVertexCount);

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
		RenderingDevice::GetRenderingDevice().CreateWICTextureFromFile(TextureResourcePath, &Data.ShaderResourceView);
	}

	if (TextureInfo.Type == "DDS")
	{
		RenderingDevice::GetRenderingDevice().CreateDDSTextureFromFile(TextureResourcePath, &Data.ShaderResourceView);
	}

	RenderingDevice::GetRenderingDevice().GetTextureDesc(Data.ShaderResourceView, &Data.TextureDESC);

	AddTextureData(TextureInfo.Name, Data);
}

FPTextureArrayData& FPAssetManager::GetTextureArrayData(std::string TextureArrayName)
{
	return this->TextureArrayData[TextureArrayName];
}

void FPAssetManager::AddTextureArrayData(std::string TextureArrayName, FPTextureArrayData& TextureArrayData)
{
	this->TextureArrayData[TextureArrayName] = TextureArrayData;
}

bool FPAssetManager::HasTextureArrayData(std::string TextureArrayName)
{
	if (TextureArrayData.count(TextureArrayName) > 0)
	{
		return true;
	}

	return false;
}

void FPAssetManager::MakeTextureArray(FPTextureArrayInfo& TextureInfo)
{
	FPTextureArrayData Data;

	//1. Texture Array 기본 속성이 적합한지 판단
	if (TextureInfo.ArraySize == 0 || TextureInfo.ArraySize != TextureInfo.SourceTextures.size() || TextureInfo.ArraySize > 2048)
	{
		std::cout << "배열 크기가 0이거나 배열크기가 텍스처 배열 최대개수(2048)를 초과했거나 배열 크기와 리소스 개수 불일치!" << "\n";
		return;
	}

	//2. 해상도 범위 체크
	if (TextureInfo.Width <= 0 || TextureInfo.Height <= 0 || TextureInfo.Width > 16384 || TextureInfo.Height > 16384)
	{
		std::cout << "해상도 범위 오류 under 0 or over 16384" << "\n";
		return;
	}

	//원본 텍스처 배열 보관
	std::vector<void*> CheckTexture;
	std::vector<UINT> TextureMiplevels;
	std::vector<void*> TextureSources;
	void* BaseDesc = nullptr;
	void* BaseSRVDesc = nullptr;
	UINT MinMaplevels;

	//Texture Data에서 Resource를 찾아서 TextureSources 배열에 넣음 (가능한 경우인지 체크 + 최소 Mip level을 찾음)
	for (int i = 0; i < TextureInfo.ArraySize; ++i)
	{
		CheckTexture.push_back(GetTextureData(TextureInfo.SourceTextures[i]).ShaderResourceView);
	}

	RenderingDevice::GetRenderingDevice().CheckTextureArrayData(TextureInfo.Width, TextureInfo.Height, &CheckTexture, &TextureMiplevels ,&TextureSources, &MinMaplevels, &BaseDesc, &BaseSRVDesc);

	//Texture2DArray 생성
	RenderingDevice::GetRenderingDevice().CreateTextureArray2D(TextureInfo.Width, TextureInfo.Height, TextureInfo.ArraySize , &TextureSources, &TextureMiplevels ,BaseDesc, BaseSRVDesc, MinMaplevels,  &Data.TextureArraySRV);

	AddTextureArrayData(TextureInfo.Name, Data);
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

FPRenderTargetTexture2DInfo& FPAssetManager::GetRenderTargetTexture2DInfoData(std::string RenderTargetTexture2DName)
{
	return LoadedRenderTargetTexture2DData[RenderTargetTexture2DName];
}

void FPAssetManager::AddRenderTargetTexture2DData(std::string RenderTargetTexture2DName, FPRenderTargetTexture2DInfo& RenderTargetTexture2DData)
{
	this->LoadedRenderTargetTexture2DData[RenderTargetTexture2DName] = RenderTargetTexture2DData;
}

bool FPAssetManager::HasRenderTargetTexture2DData(std::string RenderTargetTexture2DName)
{
	if (LoadedRenderTargetTexture2DData.count(RenderTargetTexture2DName) > 0)
	{
		return true;
	}

	return false;
}

FPRenderTargetData& FPAssetManager::GetRenderTargetData(std::string RenderTargetName)
{
	return RenderTargetData[RenderTargetName];
}

void FPAssetManager::AddRenderTargetData(std::string RenderTargetName, FPRenderTargetData& RenderTargetData)
{
	this->RenderTargetData[RenderTargetName] = RenderTargetData;
}

bool FPAssetManager::HasRenderTargetData(std::string RenderTargetName)
{
	if (RenderTargetData.count(RenderTargetName) > 0)
	{
		return true;
	}

	return false;
}

void FPAssetManager::MakeRenderTarget(FPRenderTargetTexture2DInfo& RenderTargetInfo)
{
	FPRenderTargetData RenderTargetResourceData;
	RenderingDevice::GetRenderingDevice().CreateRTTex(RenderTargetInfo.SizeX, RenderTargetInfo.SizeY, RenderTargetInfo.MSAA, RenderTargetInfo.MipMap, RenderTargetInfo.RenderTragetFormat, &RenderTargetResourceData.Texture, &RenderTargetResourceData.ResolveTexture);
	RenderingDevice::GetRenderingDevice().CreateRTView(RenderTargetInfo.RenderTragetFormat, RenderTargetInfo.MSAA, RenderTargetResourceData.Texture, &RenderTargetResourceData.RenderTargetView);
	RenderingDevice::GetRenderingDevice().CreateRTSRView(RenderTargetInfo.RenderTragetFormat, RenderTargetResourceData.ResolveTexture, &RenderTargetResourceData.ShaderResourceView);
	RenderTargetData[RenderTargetInfo.Name] = RenderTargetResourceData;
}

FPDepthStencilInfo& FPAssetManager::GetDepthStencilInfoData(std::string DepthStencilName)
{
	return LoadedDepthStencilData[DepthStencilName];
}

void FPAssetManager::AddDepthStencilInfoData(std::string DepthStencilName, FPDepthStencilInfo& DepthStencilyData)
{
	this->LoadedDepthStencilData[DepthStencilName] = DepthStencilyData;
}

bool FPAssetManager::HasDepthStencilInfoData(std::string DepthStencilName)
{
	if (LoadedDepthStencilData.count(DepthStencilName) > 0)
	{
		return true;
	}

	return false;
}

FPDepthStencilData& FPAssetManager::GetDepthStencilData(std::string DepthStencilName)
{
	return DepthStencilData[DepthStencilName];
}

void FPAssetManager::AddtDepthStencilData(std::string DepthStencilName, FPDepthStencilData& TextureData)
{
	this->DepthStencilData[DepthStencilName] = TextureData;
}

bool FPAssetManager::HastDepthStencilData(std::string DepthStencilName)
{
	if (DepthStencilData.count(DepthStencilName) > 0)
	{
		return true;
	}

	return false;
}

void FPAssetManager::MaketDepthStencil(FPDepthStencilInfo& TextureInfo)
{
	FPDepthStencilData DepthStencilResource;

	DepthStencilData[TextureInfo.Name] = DepthStencilResource;
}

FPDepthStencilStateInfo& FPAssetManager::GetDepthStencilStateInfoData(std::string DepthStencilStateName)
{
	return LoadedDepthStencilStateData[DepthStencilStateName];
}

void FPAssetManager::AddDepthStencilStateInfoData(std::string DepthStencilStateName, FPDepthStencilStateInfo& DepthStencilStateData)
{
	this->LoadedDepthStencilStateData[DepthStencilStateName] = DepthStencilStateData;
}

bool FPAssetManager::HasDepthStencilStateInfoData(std::string DepthStencilStateName)
{
	if (LoadedDepthStencilStateData.count(DepthStencilStateName) > 0)
	{
		return true;
	}

	return false;
}

FPDepthStencilStateData& FPAssetManager::GetDepthStencilStateData(std::string DepthStencilStateName)
{
	return DepthStencilStateData[DepthStencilStateName];
}

void FPAssetManager::AddDepthStencilStateData(std::string DepthStencilStateName, FPDepthStencilStateData& DepthStencilStateData)
{
	this->DepthStencilStateData[DepthStencilStateName] = DepthStencilStateData;
}

bool FPAssetManager::HasDepthStencilStateData(std::string DepthStencilStateName)
{
	if (DepthStencilStateData.count(DepthStencilStateName) > 0)
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
	Material->SetTwoSided(MaterialData.TwoSided);
	
	const FPVertexShaderData& VS = GetVertexShaderData(MaterialData.VertexShader);
	const FPPixelShaderData& PS = GetPixelShaderData(MaterialData.PixelShader);

	//Relflection 분석을 위한 구조체

	//input
	void* VScode = std::get<1>(GetVertexShader(VS.Name));
	void* PScode = std::get<1>(GetPixelShader(PS.Name));
	std::set<std::pair<FPShaderType, std::string> > ManagedCBs;			//실수형 파라미터의 정보를 담은 자료구조
	for (const FPFloatParameterData& P : MaterialData.Parameters) ManagedCBs.insert({ P.Type, P.ConstantBufferName });

	//Reflection 결과 pointer를 활용해 객체에 바로 데이터를 갱신
	FPShaderReflectionData* ReflectionData = Material->GetReflectionData();
	FPConstantBufferRenderData* VertexConstantBuffer = Material->GetVertexConstantBufferRenderData();
	FPConstantBufferRenderData* PixelConstantBuffer = Material->GetPixelConstantBufferRenderData();

	HRESULT hr = S_OK;
	//Reflection 분석은 RenderingDevice가 담당
	hr = RenderingDevice::GetRenderingDevice().ReflectionShader(VScode, PScode, ManagedCBs, ReflectionData, VertexConstantBuffer, PixelConstantBuffer);
	if (FAILED(hr)) { std::cout << "마테리얼 셰이더 리플렉션 Fail!" << "\n"; }

	for (const FPFloatParameterData& P : MaterialData.Parameters)
	{
		if (!Material->SetParameter(P.Type, P.ConstantBufferName, P.ParameterName, P.Values))
		{
			std::cout << "Material parameter missing/type mismatch: "
				<< P.ConstantBufferName
				<< "." << P.ParameterName << "\n";
			delete Material;
			return nullptr;
		}
	}

	for (const FPTextureParameterData& T : MaterialData.NamedTextures)
	{
		const bool AssetFound = (T.TextureType == Texture2D) ? HasTextureData(T.Name) : HasTextureArrayData(T.Name);
		if (!AssetFound)
		{
			std::cout << "Material texture asset not loaded: " << T.Name << "\n";
			delete Material;
			return nullptr;
		}
		if (!Material->SetNamedTexture(T.Type, T.Parameter, T.Name,T.TextureType, T.ArrayIndex))
		{
			std::cout << "Material texture failed : " << T.Name << "\n";
			delete Material;
			return nullptr;
		}
	}

	for (const FPSamplerParameterData& S : MaterialData.NamedSamplers)
	{
		if (!Material->SetNamedSampler(S.Type, S.Parameter, S.SamplerType, S.ArrayIndex))
		{
			std::cout << "Material sampler failed \n";
			delete Material;
			return nullptr;
		}
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











