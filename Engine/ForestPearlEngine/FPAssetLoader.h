#pragma once
#include <string>
#include <unordered_map>
#include "Renderers/FPRenderingCommon.h"
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"

class FPAssetLoader : public FPGameInstanceSubSystem
{
	private:

	public:
		FPAssetLoader() = default;
		~FPAssetLoader() = default;

		//GameSetting
		void LoadGameSetting(std::string GameSettingPath);

		//Level Load
		void LoadLevelData(std::string LevelName, std::string LevelPath, AssetOwner EngineAsset);

		//VertexShader
		void LoadVertexShader(std::string ShaderPath, AssetOwner EngineAsset);

		//PixelShader
		void LoadPixelShader(std::string ShaderPath, AssetOwner EngineAsset);

		//StaticMesh Load
		void LoadStaticMesh(std::string StaticMeshPath, AssetOwner EngineAsset);

		//Texture Load
		void LoadTexture(std::string TexturePath, AssetOwner EngineAsset);

		//Texture 2D Array Load
		void LoadTexture2DArray(std::string Texture2DArrayPath, AssetOwner EngineAsset);

		//Material Load
		void LoadMaterial(std::string MaterialPath, AssetOwner EngineAsset);
};