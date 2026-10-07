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
		std::unordered_map<std::string, std::vector<FPVertexBufferData> > MeshVertexBuffer;

		std::unordered_map<std::string, std::vector<FPMeshData> > LoadedMeshData;
		
		std::unordered_map<std::string, FPStaticMeshData> LoadedStaticMeshData;

		std::unordered_map<std::string, std::vector<FPActorData> > LevelData;

		std::unordered_map<std::string, std::string> GameModeData;

		std::unordered_map<std::string, std::tuple<void*, void*, void*> > VertexShaderData;
		std::unordered_map<std::string, std::pair<void*, void*> > PixelShaderData;

	public:
		FPAssetManager() = default;
		~FPAssetManager() = default;

		//Mesh
		void AddMeshData(std::string FbxPath, FPMeshData* MeshData);

		void AddVertexBuffer(std::string FbxPath, void* VertexBuffer, int VBSize, int Stride, int Offset);

		std::vector<FPVertexBufferData> GetVertexBuffer(std::string MeshPath);

		std::vector<FPMeshData> GetMeshData(std::string FbxPath);

		//StaticMesh
		FPStaticMesh* GetStaticMeshData(std::string StaticMeshName);

		void AddStaticMeshData(std::string StaticMeshName, FPStaticMeshData* StaticMeshData);

		bool HasStaticMeshData(std::string StaticMeshName);

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

		std::tuple<void*, void*, void*> GetVertexShader(std::string ShaderPath);
		std::pair<void*, void*> GetPixelShader(std::string ShaderPath);
};