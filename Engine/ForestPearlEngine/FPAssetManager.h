#pragma once
#include <string>
#include <unordered_map>
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"
#include "Define/FPDataDefine.h"

class FPAssetManager : public FPGameInstanceSubSystem
{
	private:
		std::unordered_map<std::string, std::vector<std::pair<int, int> > > MeshMap;

		std::unordered_map<std::string, std::vector<FPMeshData> > LoadedMeshData;

		std::unordered_map<std::string, std::vector<FPActorData> > LevelData;

		std::unordered_map<std::string, std::string> GameModeData;

	public:
		FPAssetManager() = default;
		~FPAssetManager() = default;

		//Mesh
		void AddMeshData(std::string FbxPath, FPMeshData* MeshData);

		void AddVertexBuffer(std::string FbxPath, int VBIndex, int VBSize);

		std::vector<std::pair<int, int> > GetVertexBuffer(std::string MeshPath);

		std::vector<FPMeshData> GetMeshData(std::string FbxPath);

		//Level
		bool HasLevelData(std::string LevelName);

		void AddLevelData(std::string LevelName, FPActorData* ActorData);

		std::vector<FPActorData>& GetLevelData(std::string LevelName);

		//GameMode
		void AddGameModeData(std::string LevelName, std::string GameModeName);
		std::string GetGameModeData(std::string LevelName);
};