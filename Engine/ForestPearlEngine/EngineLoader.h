#pragma once
#include "FPGameInstance.h"
#include "FPAssetLoader.h"

void LoadEngineAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//Default Vertex/Pixel Shader
	AssetLoader->LoadVertexShader("DefaultVertexShader.vso", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultPixelShader.pso", AssetOwner::Engine);
};