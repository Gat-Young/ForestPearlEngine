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

	//Primitive Model
	AssetLoader->LoadFbxData("Primitive_Model/Cube.fbx", AssetOwner::Engine);
	AssetLoader->LoadFbxData("Primitive_Model/Sphere.fbx", AssetOwner::Engine);

	//StaticMesh Load
	AssetLoader->LoadStaticMesh("Cube_StaticMesh", "Cube_StaticMesh.json", AssetOwner::Engine);
	AssetLoader->LoadStaticMesh("Sphere_StaticMesh", "Sphere_StaticMesh.json", AssetOwner::Engine);
};