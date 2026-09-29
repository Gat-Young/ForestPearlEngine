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
std::vector<FPVertexBufferData> FPAssetManager::GetVertexBuffer(std::string MeshPath)
{
	if (MeshVertexBuffer.count(MeshPath) <= 0)
	{
		std::cout << MeshPath << "의 VB 데이터가 없습니다." << "\n";

		return {};
	}

	return MeshVertexBuffer[MeshPath];
}

void FPAssetManager::AddMeshData(std::string FbxPath, FPMeshData* MeshData)
{
	LoadedMeshData[FbxPath].push_back(std::move(*MeshData));
}

void FPAssetManager::AddVertexBuffer(std::string FbxPath, void* VertexBuffer, int VBSize, int Stride, int Offset)
{
	FPVertexBufferData VBData;
	VBData.VertexBuffer = VertexBuffer;
	VBData.Size = VBSize;
	VBData.Stride = Stride;
	VBData.Offset = Offset;

	MeshVertexBuffer[FbxPath].push_back(VBData);
}

std::vector<FPMeshData> FPAssetManager::GetMeshData(std::string FbxPath)
{
	return LoadedMeshData[FbxPath];
}


////////////////////////////
//
// StaticMesh
//
FPStaticMesh* FPAssetManager::GetStaticMeshData(std::string StaticMeshName)
{
	FPStaticMeshData StaticMeshData = LoadedStaticMeshData[StaticMeshName];
	FPStaticMesh* StaticMesh = new FPStaticMesh(StaticMeshData.MeshPath, StaticMeshData.MeshTopology);

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

void FPAssetManager::AddVertexShader(std::string ShaderPath, void* VertexShader, void* VSCode)
{
	VertexShaderData[ShaderPath] = { VertexShader, VSCode };
}

void FPAssetManager::AddPixelShader(std::string ShaderPath, void* PixelShader, void* PSCode)
{
	PixelShaderData[ShaderPath] = { PixelShader, PSCode };
}

std::pair<void*, void*> FPAssetManager::GetVertexShader(std::string ShaderPath)
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











