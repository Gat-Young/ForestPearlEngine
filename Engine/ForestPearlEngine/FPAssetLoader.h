#pragma once
#include <string>
#include <unordered_map>
#include "Libraries/Ufbx/ufbx.h"
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"

class FPAssetLoader : public FPGameInstanceSubSystem
{
	private:
		std::string ConvertUfbxString(ufbx_string String);
		struct FPMeshData ConvertUfbxMesh(const ufbx_mesh* Mesh, const ufbx_node* Node);

	public:
		FPAssetLoader() = default;
		~FPAssetLoader() = default;

		void LoadVertexBuffer(std::string MeshPath);

		int MakeVertexBuffer(std::vector<struct VERTEX> Mesh);

		void LoadFbxData(std::string FbxPath);

		void LoadLevelData(std::string LevelName, std::string LevelPath);
};