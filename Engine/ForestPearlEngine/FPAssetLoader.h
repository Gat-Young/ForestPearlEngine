#pragma once
#include <string>
#include <unordered_map>
#include "Define/FPDataDefine.h"
#include "Libraries/Ufbx/ufbx.h"
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"

class FPAssetLoader : public FPGameInstanceSubSystem
{
	private:
		std::string ConvertUfbxString(ufbx_string String);
		struct FPMeshData ConvertUfbxMesh(const ufbx_mesh* Mesh, const ufbx_node* Node);

		// 색상 JSON 읽기
		COLOR_VERTEX ReadColor(const json& ColorJson);
	public:
		FPAssetLoader() = default;
		~FPAssetLoader() = default;

		void MakeGizmoVertexBuffer(std::vector<struct VERTEX>& Mesh, const std::string& GizemoMeshPath);

		//Buffer 생성
		void* MakePositionVertexBuffer(std::vector<struct POSITION_VERTEX>& Mesh);
		void* MakeColorVertexBuffer(std::vector<struct COLOR_VERTEX>& Mesh);
		void* MakeNormalVertexBuffer(std::vector<struct NORMAL_VERTEX>& Mesh);
		void* MakeUVVertexBuffer(std::vector<struct UV_VERTEX>& Mesh);
		void* MakeIndex16BitBuffer(std::vector<struct INDEX_16BIT>& Mesh);
		void* MakeIndex32BitBuffer(std::vector<struct INDEX_32BIT>& Mesh);

		void LoadFbxData(std::string FbxPath, AssetOwner EngineAsset);

		void LoadLevelData(std::string LevelName, std::string LevelPath, AssetOwner EngineAsset);

		void LoadVertexShader(std::string ShaderObjPath, AssetOwner EngineAsset);
		void LoadNormalDebugVertexShader(std::string ShaderObjPath, AssetOwner EngineAsset);

		void LoadPixelShader(std::string ShaderObjPath, AssetOwner EngineAsset);
		void LoadVertexShader(std::string ShaderPath, std::string VS_Main, std::string ShaderModel, AssetOwner EngineAsset);
		void LoadPixelShader(std::string ShaderPath, std::string PS_Main, std::string ShaderModel, AssetOwner EngineAsset);

		//StaticMesh Load
		void LoadStaticMesh(std::string StaticMeshName, std::string StaticMeshPath, AssetOwner EngineAsset);

		//Texture Load
		void LoadTexture(std::string TexturePath, AssetOwner EngineAsset);
		void MakeTexture(FPTextureInfo& TextureInfo);

		//Material Load
		void LoadMaterial(std::string MaterialPath, AssetOwner EngineAsset);
};