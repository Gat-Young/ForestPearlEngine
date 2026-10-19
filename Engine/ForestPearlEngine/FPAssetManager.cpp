#include "FPAssetManager.h"
#include <iostream>
#include "Utility/FPPathManager.h"
#include "FPGameInstance.h"
#include "FPGameProjectClassRegistry.h"
#include "FPMaterialInterface.h"
#include "FPStaticMesh.h"

////////////////////////////
//
// Mesh
//


void FPAssetManager::AddMeshData(std::string FbxPath, FPMeshData* MeshData)
{
	LoadedMeshData[FbxPath].push_back(std::move(*MeshData));
}

std::vector<FPMeshData> FPAssetManager::GetMeshData(std::string FbxPath)
{
	return LoadedMeshData[FbxPath];
}

void FPAssetManager::AddPositionVertexBuffer(std::string FbxPath, FPVertexBufferData& PositionVertexBuffer)
{
	MeshPositionVertexBuffer[FbxPath].push_back(PositionVertexBuffer);
}

void FPAssetManager::AddPositionVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.VB_Size = VBSize;
	VBData.VB_Cnt = VBCnt;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshPositionVertexBuffer[FbxPath].push_back(VBData);
}

bool FPAssetManager::HasPositionBuffer(std::string MeshPath)
{
	if (MeshPositionVertexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

std::vector<FPVertexBufferData> FPAssetManager::GetPositionVertexBuffer(std::string MeshPath)
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
	MeshColorVertexBuffer[FbxPath].push_back(ColorVertexBuffer);
}

void FPAssetManager::AddColorVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.VB_Size = VBSize;
	VBData.VB_Cnt = VBCnt;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshColorVertexBuffer[FbxPath].push_back(VBData);
}

bool FPAssetManager::HasColorBuffer(std::string MeshPath)
{
	if (MeshColorVertexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

std::vector<FPVertexBufferData> FPAssetManager::GetColorVertexBuffer(std::string MeshPath)
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
	MeshNormalVertexBuffer[FbxPath].push_back(NormalVertexBuffer);
}

void FPAssetManager::AddNormalVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.VB_Size = VBSize;
	VBData.VB_Cnt = VBCnt;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshNormalVertexBuffer[FbxPath].push_back(VBData);
}

bool FPAssetManager::HasNormalBuffer(std::string MeshPath)
{
	if (MeshNormalVertexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

std::vector<FPVertexBufferData> FPAssetManager::GetNormalVertexBuffer(std::string MeshPath)
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
	MeshUVVertexBuffer[FbxPath].push_back(UVVertexBuffer);
}

void FPAssetManager::AddUVVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.VB_Size = VBSize;
	VBData.VB_Cnt = VBCnt;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshUVVertexBuffer[FbxPath].push_back(VBData);
}

bool FPAssetManager::HasUVBuffer(std::string MeshPath)
{
	if (MeshUVVertexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

std::vector<FPVertexBufferData> FPAssetManager::GetUVVertexBuffer(std::string MeshPath)
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
	MeshIndexBuffer[FbxPath].push_back(PositionVertexBuffer);
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

	MeshIndexBuffer[FbxPath].push_back(IBData);
}

bool FPAssetManager::HasIndexBuffer(std::string MeshPath)
{
	if (MeshIndexBuffer.count(MeshPath) <= 0)
	{
		return false;
	}
	return true;
}

std::vector<FPIndexBufferData> FPAssetManager::GetIndexBuffer(std::string MeshPath)
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
// StaticMesh
//
FPStaticMesh* FPAssetManager::GetStaticMeshData(std::string StaticMeshName)
{
	FPStaticMeshData StaticMeshData = LoadedStaticMeshData[StaticMeshName];
	FPStaticMesh* StaticMesh = new FPStaticMesh(StaticMeshData.MeshPath);

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
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	if (ClassRegistry->HasFactory(MaterialName))
	{
		FPMaterialInterface* Material = static_cast<FPMaterialInterface*>((ClassRegistry->Create(MaterialName)).release());
		return Material;
	}
	return nullptr;
}

////////////////////////////
//
// Shader
//
bool FPAssetManager::HasVertexShader(std::string ShaderPath)
{
	if (VertexShaderData.count(ShaderPath) > 0)
	{
		return true;
	}
	return false;
}

bool FPAssetManager::HasPixelShader(std::string ShaderPath)
{
	if (PixelShaderData.count(ShaderPath) > 0)
	{
		return true;
	}
	return false;
}

void FPAssetManager::AddVertexShader(std::string ShaderPath, void* VertexShader, void* VSCode, void* VBLayout)
{
	VertexShaderData[ShaderPath] = { VertexShader, VSCode, VBLayout };
}

void FPAssetManager::AddPixelShader(std::string ShaderPath, void* PixelShader, void* PSCode)
{
	PixelShaderData[ShaderPath] = { PixelShader, PSCode };
}

std::tuple<void*, void*, void*> FPAssetManager::GetVertexShader(std::string ShaderPath)
{
	if (VertexShaderData.count(ShaderPath) <= 0)
	{
		std::cout << ShaderPath << "의 Vertex Shader 데이터가 없습니다." << "\n";

		return {};
	}
	return VertexShaderData[ShaderPath];
}

std::pair<void*, void*> FPAssetManager::GetPixelShader(std::string ShaderPath)
{
	if (PixelShaderData.count(ShaderPath) <= 0)
	{
		std::cout << ShaderPath << "의 Pixel Shader 데이터가 없습니다." << "\n";

		return {};
	}
	return PixelShaderData[ShaderPath];
}











