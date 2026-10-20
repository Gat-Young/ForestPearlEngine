#pragma once
#include "FPGameInstance.h"
#include "FPAssetLoader.h"

void LoadEngineAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//Default Vertex/Pixel Shader
	AssetLoader->LoadVertexShader("DefaultVertexShader.vso", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultPixelShader.pso", AssetOwner::Engine);

	AssetLoader->LoadVertexShader("DefaultGizmoVertexShader.vso", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultGizmoPixelShader.pso", AssetOwner::Engine);

	AssetLoader->LoadNormalDebugVertexShader("NormalDebugVertexShader.vso", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("NormalDebugPixelShader.pso", AssetOwner::Engine);

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


	//이건 나중에 Engine에서 Lazy 하게 생성
	//Primitive Model
	AssetLoader->LoadFbxData("Primitive_Model/Cube.fbx", AssetOwner::Engine);
	AssetLoader->LoadFbxData("Primitive_Model/Sphere.fbx", AssetOwner::Engine);
	AssetLoader->LoadFbxData("Primitive_Model/Plane.fbx", AssetOwner::Engine);
};