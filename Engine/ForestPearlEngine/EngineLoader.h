#pragma once
#include "FPGameInstance.h"
#include "FPAssetLoader.h"

void LoadEngineAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//Default Vertex/Pixel Shader
	AssetLoader->LoadVertexShader("DefaultShader.fx", "VS_Main", "vs_5_0", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultShader.fx", "PS_Main", "ps_5_0", AssetOwner::Engine);
};