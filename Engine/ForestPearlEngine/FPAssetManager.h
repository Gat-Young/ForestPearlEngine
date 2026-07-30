#pragma once
#include <string>
#include <unordered_map>
#include "Libraries/Ufbx/ufbx.h"
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"

//정점 구조체
struct VERTEX
{
	float x, y, z;		//좌표 Position
	float r, g, b, a;	//색상 Diffuse Color
};

//메시 데이터 구조체
struct FPMeshData
{
	std::string Name;
	std::vector<VERTEX> Vertices;
};

struct FPActorData
{
	std::string ClassName;
	std::string ActorName;
	float Location_x, Location_y, Location_z;
	float Rotation_x, Rotation_y, Rotation_z;
	float Scale_x, Scale_y, Scale_Z;
};

class FPAssetManager : public FPGameInstanceSubSystem
{
	private:
		std::unordered_map<std::string, std::vector<std::pair<int, int> > > MeshMap;

		std::string AssetsPath = "../../Engine/ForestPearlEngine/Assets/";

		std::unordered_map<std::string, std::vector<FPMeshData> > LoadedMeshData;

		std::unordered_map<std::string, std::vector<FPActorData> > LevelData;

		std::unordered_map<std::string, std::string> GameModeData;

		std::string ConvertUfbxString(ufbx_string String);
		FPMeshData ConvertUfbxMesh(const ufbx_mesh* Mesh, const ufbx_node* Node);

	public:
		FPAssetManager() = default;
		~FPAssetManager() = default;

		std::vector<std::pair<int, int> > LoadVertexBuffer(std::string MeshPath);

		int MakeVertexBuffer(std::vector<VERTEX> Mesh);

		void LoadFbxData(std::string FbxPath);

		void LoadLevelData(std::string LevelName, std::string LevelPath);

		std::vector<FPActorData>& GetLevelData(std::string LevelName);

		std::string GetGameModeData(std::string LevelName);
};