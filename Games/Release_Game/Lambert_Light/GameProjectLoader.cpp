#include "ForestPearlEngine/GameProjectLoader.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"
#include "Cube.h"
#include "Grid.h"
#include "Axis.h"

void RegistProjectName()
{
	FPPathManager::Get().Initialize("Lambert_Light");
	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	GameProjectSetting->SetWinWidth(960);
	GameProjectSetting->SetWinHeight(600);
	GameProjectSetting->ProjectSetting();
}

void LoadLevel()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadLevelData("Lambert_Light", "Lambert_Light.json", AssetOwner::User);
}

void LoadClassRegist()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<GameMode>("GameMode");
	ClassRegistry->Register<GameController>("GameController");
	ClassRegistry->Register<UI>("UI");
	ClassRegistry->Register<Cube>("Cube");
	ClassRegistry->Register<Grid>("Grid");
	ClassRegistry->Register<Axis>("Axis");


	//Material Class µî·Ï
}

void LoadAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
}

std::string ReturnStartLevel()
{
	return "Lambert_Light";
}