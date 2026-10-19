#pragma once
#include <string>
#include <unordered_map>
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"
#include "Define/FPDataDefine.h"

class FPStaticMesh;
class FPMaterialInterface;

class FPAssetManager : public FPGameInstanceSubSystem
{
private:
	std::unordered_map<std::string, std::vector<FPVertexBufferData> > MeshPositionVertexBuffer;
	std::unordered_map<std::string, std::vector<FPVertexBufferData> > MeshNormalVertexBuffer;
	std::unordered_map<std::string, std::vector<FPVertexBufferData> > MeshUVVertexBuffer;
	std::unordered_map<std::string, std::vector<FPVertexBufferData> > MeshColorVertexBuffer;
	std::unordered_map<std::string, std::vector<FPIndexBufferData> > MeshIndexBuffer;

	std::unordered_map<std::string, std::vector<FPMeshData> > LoadedMeshData;

	std::unordered_map<std::string, FPStaticMeshData> LoadedStaticMeshData;

	std::unordered_map<std::string, std::vector<FPActorData> > LevelData;

	std::unordered_map<std::string, std::string> GameModeData;

	std::unordered_map<std::string, std::tuple<void*, void*, void*> > VertexShaderData;
	std::unordered_map<std::string, std::pair<void*, void*> > PixelShaderData;

	std::unordered_map<std::string, FPTextureData> TextureData;

public:
	FPAssetManager() = default;
	~FPAssetManager() = default;

	//Mesh
	void AddMeshData(std::string FbxPath, FPMeshData* MeshData);

	std::vector<FPMeshData> GetMeshData(std::string FbxPath);


	//각 필요한 정점 버퍼에 값 추가

	//Position
	void AddPositionVertexBuffer(std::string FbxPath, FPVertexBufferData& PositionVertexBuffer);
	void AddPositionVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset);
	bool HasPositionBuffer(std::string MeshPath);
	std::vector<FPVertexBufferData> GetPositionVertexBuffer(std::string MeshPath);

	//Color
	void AddColorVertexBuffer(std::string FbxPath, FPVertexBufferData& ColorVertexBuffer);
	void AddColorVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset);
	bool HasColorBuffer(std::string MeshPath);
	std::vector<FPVertexBufferData> GetColorVertexBuffer(std::string MeshPath);

	//Normal
	void AddNormalVertexBuffer(std::string FbxPath, FPVertexBufferData& NormalVertexBuffer);
	void AddNormalVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset);
	bool HasNormalBuffer(std::string MeshPath);
	std::vector<FPVertexBufferData> GetNormalVertexBuffer(std::string MeshPath);

	//UV
	void AddUVVertexBuffer(std::string FbxPath, FPVertexBufferData& UVVertexBuffer);
	void AddUVVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset);
	bool HasUVBuffer(std::string MeshPath);
	std::vector<FPVertexBufferData> GetUVVertexBuffer(std::string MeshPath);

	//Index
	void AddIndexBuffer(std::string FbxPath, FPIndexBufferData& PositionVertexBuffer);
	void AddIndexBuffer(std::string FbxPath, void* IndexBuffer, unsigned long IBSize, unsigned long IBCnt, unsigned long FaceCnt, unsigned long Stride, unsigned long Offset);
	bool HasIndexBuffer(std::string MeshPath);
	std::vector<FPIndexBufferData> GetIndexBuffer(std::string MeshPath);

	//StaticMesh
	FPStaticMesh* GetStaticMeshData(std::string StaticMeshName);

	void AddStaticMeshData(std::string StaticMeshName, FPStaticMeshData* StaticMeshData);

	bool HasStaticMeshData(std::string StaticMeshName);


	//Texture
	FPTextureData& GetTextureData(std::string TextureName);

	void AddTextureData(std::string TextureName, FPTextureData& TextureData);

	bool HasTextureData(std::string TextureName);

	//Level
	bool HasLevelData(std::string LevelName);

	void AddLevelData(std::string LevelName, FPActorData* ActorData);

	std::vector<FPActorData>& GetLevelData(std::string LevelName);

	//GameMode
	void AddGameModeData(std::string LevelName, std::string GameModeName);
	std::string GetGameModeData(std::string LevelName);

	//Material
	FPMaterialInterface* MakeMaterial(std::string MaterialName);

	//Shader
	bool HasVertexShader(std::string ShaderPath);
	bool HasPixelShader(std::string ShaderPath);

	void AddVertexShader(std::string ShaderPath, void* VertexShader, void* VSCode, void* VBLayout);
	void AddPixelShader(std::string ShaderPath, void* PixelShader, void* PSCode);

	//VertexShader , VSCode, VBLayout
	std::tuple<void*, void*, void*> GetVertexShader(std::string ShaderPath);

	//PixelShader, PSCode
	std::pair<void*, void*> GetPixelShader(std::string ShaderPath);
};