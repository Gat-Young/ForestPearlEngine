#pragma once
#include <string>
#include <unordered_map>
#include "Libraries/Ufbx/ufbx.h"
#include "../ForestPearlEngine/Define/FPMath.h"

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

class AssetManager
{
	private:
		std::unordered_map<std::string, std::vector<std::pair<int, int> > > MeshMap;

		std::string AssetsPath = "../../Engine/ForestPearlEngine/Assets/";

		std::unordered_map<std::string, std::vector<FPMeshData> > LoadedMeshData;

		AssetManager();
		~AssetManager() = default;

		std::string ConvertUfbxString(ufbx_string String);
		FPMeshData ConvertUfbxMesh(const ufbx_mesh* Mesh, const ufbx_node* Node);

	public:
		//Single Tone
		static AssetManager& Get()
		{
			static AssetManager Instance;
			return Instance;
		}

		std::vector<std::pair<int, int> > LoadVertexBuffer(std::string MeshPath);

		int MakeVertexBuffer(std::vector<VERTEX> Mesh);

		void LoadFbxData(std::string FbxPath);
};