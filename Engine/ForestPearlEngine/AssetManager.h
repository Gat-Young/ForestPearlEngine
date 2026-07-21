#pragma once
#include <string>
#include <unordered_map>
#include "../ForestPearlEngine/Define/FPMath.h"

struct FPMesh
{
	FPVector3 vPos;
	float r, g, b, a;
};

struct VERTEX;

class AssetManager
{
	private:
		std::unordered_map<std::string, int> MeshMap;

		std::string AssetsPath = "../../Engine/ForestPearlengine/Assets/";

		//Dummy Mesh <- 추후에 로드된 Mesh Data 사용
		std::unordered_map<std::string, std::vector<FPMesh> > DummyMesh;

		AssetManager();
		~AssetManager() = default;

		std::vector<VERTEX> ChangeVERTEX(std::vector<FPMesh> Mesh);

	public:
		//Single Tone
		static AssetManager& Get()
		{
			static AssetManager Instance;
			return Instance;
		}

		std::pair<int, int> LordVertexBuffer(std::string MeshPath);

		int MakeVertexBuffer(std::vector<FPMesh> Mesh);

		void LoadFbxData(std::string FbxPath, std::string FileName);
};