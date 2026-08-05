#include "ForestPearlEngine/GameProjectLoader.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"
#include "Triangle.h"
#include "GameCamera.h"

#include <iostream>

void RegistProjectName()
{
	FPPathManager::Get().Initialize("ShaderCode_Triangle");
}

void LoadLevel()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	AssetManager->LoadLevelData("ShaderCode_Triangle", "ShaderCode_Triangle.json");

}

void LoadClassRegist()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<GameMode>("GameMode");
	ClassRegistry->Register<GameController>("GameController");
	ClassRegistry->Register<UI>("UI");
	ClassRegistry->Register<Triangle>("Triangle");
	ClassRegistry->Register<GameCamera>("GameCamera");
}

void LoadAssets()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	AssetManager->LoadFbxData("Triangle/Test_Triangle.fbx");
}

std::string ReturnStartLevel()
{
	return "ShaderCode_Triangle";
}