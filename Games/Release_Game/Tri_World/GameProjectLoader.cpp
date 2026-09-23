#include "ForestPearlEngine/GameProjectLoader.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"
#include "Player.h"
#include "GameCamera.h"
#include "Grid.h"
#include "Axis.h"
#include "Terrain.h"
#include "Tree.h"
#include "Windmill.h"
#include "WindmillWing.h"
#include "TripleWindmillWing.h"

void RegistProjectName()
{
	FPPathManager::Get().Initialize("Tri_World");
	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	GameProjectSetting->SetWinWidth(960);
	GameProjectSetting->SetWinHeight(600);
	GameProjectSetting->ProjectSetting();
}

void LoadLevel()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadLevelData("TriWorld", "TriWorld.json", AssetOwner::User);
}

void LoadClassRegist()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<GameMode>("GameMode");
	ClassRegistry->Register<GameController>("GameController");
	ClassRegistry->Register<UI>("UI");
	ClassRegistry->Register<Player>("Player");
	ClassRegistry->Register<Tree>("Tree");
	//ClassRegistry->Register<GameCamera>("GameCamera");
	ClassRegistry->Register<Grid>("Grid");
	ClassRegistry->Register<Axis>("Axis");
	ClassRegistry->Register<Terrain>("Terrain");
	ClassRegistry->Register<Windmill>("Windmill");
	ClassRegistry->Register<WindmillWing>("WindmillWing");
	ClassRegistry->Register<TripleWindmillWing>("TripleWindmillWing");
}

void LoadAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadFbxData("ToonLink/ToonLinkTriangle.fbx", AssetOwner::User);
	AssetLoader->LoadFbxData("ToonLink/ToonLink.fbx", AssetOwner::User);
	AssetLoader->LoadFbxData("Terrain/Terrain.fbx", AssetOwner::User);
	AssetLoader->LoadFbxData("Tree/Tree.fbx", AssetOwner::User);
	AssetLoader->LoadFbxData("Windmill/Windmill_Body.fbx", AssetOwner::User);
	AssetLoader->LoadFbxData("Windmill/Windmill_Wing.fbx", AssetOwner::User);
}

std::string ReturnStartLevel()
{
	return "TriWorld";
}