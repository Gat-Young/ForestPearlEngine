#pragma once
#include <string>
#include <unordered_map>
#include "Libraries/Ufbx/ufbx.h"
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"

enum AssetOwner
{
	Engine,
	User
};

class FPAssetLoader : public FPGameInstanceSubSystem
{
	private:
		std::string ConvertUfbxString(ufbx_string String);
		struct FPMeshData ConvertUfbxMesh(const ufbx_mesh* Mesh, const ufbx_node* Node);

	public:
		FPAssetLoader() = default;
		~FPAssetLoader() = default;

		void* MakeVertexBuffer(std::vector<struct VERTEX> Mesh);

		void LoadFbxData(std::string FbxPath, AssetOwner EngineAsset);

		void LoadLevelData(std::string LevelName, std::string LevelPath, AssetOwner EngineAsset);

		void LoadVertexShader(std::string ShaderObjPath, AssetOwner EngineAsset);
		void LoadPixelShader(std::string ShaderObjPath, AssetOwner EngineAsset);
		void LoadVertexShader(std::string ShaderPath, std::string VS_Main, std::string ShaderModel, AssetOwner EngineAsset);
		void LoadPixelShader(std::string ShaderPath, std::string PS_Main, std::string ShaderModel, AssetOwner EngineAsset);

		void LoadStaticMesh(std::string StaticMeshName, std::string StaticMeshPath, AssetOwner EngineAsset);
};