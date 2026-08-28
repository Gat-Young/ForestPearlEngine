#include "ForestPearlEngine/GameProjectLoader.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"
#include "Triangle.h"
#include "GameCamera.h"
#include "MoveVertexConstBufferMaterial.h"

#include <iostream>

void RegistProjectName()
{
	FPPathManager::Get().Initialize("ShaderCode_Triangle");

	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	GameProjectSetting->SetWinWidth(1800);
	GameProjectSetting->SetWinHeight(600);
	GameProjectSetting->ProjectSetting();

}

void LoadLevel()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadLevelData("ShaderCode_Triangle", "ShaderCode_Triangle.json", AssetOwner::User);

}

void LoadClassRegist()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<GameMode>("GameMode");
	ClassRegistry->Register<GameController>("GameController");
	ClassRegistry->Register<UI>("UI");
	ClassRegistry->Register<Triangle>("Triangle");
	ClassRegistry->Register<GameCamera>("GameCamera");
	//ClassRegistry->Register<MoveVertexConstBufferMaterial>("MoveVertexConstBufferMaterial");
}

void LoadAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadFbxData("Triangle/Test_Triangle.fbx", AssetOwner::User);
	AssetLoader->LoadFbxData("Triangle/Test_Clip_Triangle.fbx", AssetOwner::User);

	//AssetLoader->LoadVertexShader("Demo.fx","VS_Main", "vs_5_0", AssetOwner::User);
	//AssetLoader->LoadVertexShader("2CB.fx", "VS_Main", "vs_5_0", AssetOwner::User);

	//AssetLoader->LoadPixelShader("Demo.fx", "PS_Main", "ps_5_0", AssetOwner::User);
	//AssetLoader->LoadPixelShader("2CB.fx", "PS_Main", "ps_5_0", AssetOwner::User);

	AssetLoader->LoadVertexShader("Demo.vso", AssetOwner::User);
	AssetLoader->LoadPixelShader("Demo.pso", AssetOwner::User);
}

std::string ReturnStartLevel()
{
	return "ShaderCode_Triangle";
}