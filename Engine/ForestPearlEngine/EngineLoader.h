#pragma once
#include "FPGameInstance.h"
#include "FPAssetLoader.h"

void LoadEngineAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//BuiltIn Mesh Load
	AssetLoader->LoadBuiltInStaticMesh("Quad2D_Mesh.json");

	//BuiltIn Vertex/Pixel Shader Load
	AssetLoader->LoadBuiltInVertexShader("DefaultQuad2DVertexShader.json");
	AssetLoader->LoadBuiltInPixelShader("DefaultQuad2DPixelShader.json");

	//Default Vertex/Pixel Shader
	AssetLoader->LoadVertexShader("DefaultVertexShader.json", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultPixelShader.json", AssetOwner::Engine);
	AssetLoader->LoadVertexShader("DefaultGizmoVertexShader.json", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultGizmoPixelShader.json", AssetOwner::Engine);
	AssetLoader->LoadVertexShader("NormalDebugVertexShader.json", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("NormalDebugPixelShader.json", AssetOwner::Engine);

	//StaticMesh Load
	AssetLoader->LoadStaticMesh("Cube_StaticMesh.json", AssetOwner::Engine);
	AssetLoader->LoadStaticMesh("Sphere_StaticMesh.json", AssetOwner::Engine);
	AssetLoader->LoadStaticMesh("Plane_StaticMesh.json", AssetOwner::Engine);

	//Texture Load
	AssetLoader->LoadTexture("Default_Texture512.json", AssetOwner::Engine);
	AssetLoader->LoadTexture("Checker_bmp.json", AssetOwner::Engine);
	AssetLoader->LoadTexture("Checker_dds.json", AssetOwner::Engine);

	//Material Load
	AssetLoader->LoadMaterial("DefaultMaterial.json", AssetOwner::Engine);

	//DepthStencilState Load
	AssetLoader->LoadDepthStencilState("DS_DEPTH_ON.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_OFF.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_WRITE_OFF.json", AssetOwner::Engine);
};