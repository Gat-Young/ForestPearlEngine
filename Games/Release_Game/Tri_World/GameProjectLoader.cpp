#include "ForestPearlEngine/GameProjectLoader.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"
#include "Player.h"
#include "Grid.h"
#include "Axis.h"
#include "Terrain.h"
#include "Tree.h"
#include "Windmill.h"
#include "WindmillWing.h"
#include "TripleWindmillWing.h"
#include "TripleWingWindmill.h"

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
	ClassRegistry->Register<Grid>("Grid");
	ClassRegistry->Register<Axis>("Axis");
	ClassRegistry->Register<Terrain>("Terrain");
	ClassRegistry->Register<Windmill>("Windmill");
	ClassRegistry->Register<WindmillWing>("WindmillWing");
	ClassRegistry->Register<TripleWindmillWing>("TripleWindmillWing");
	ClassRegistry->Register<TripleWingWindmill>("TripleWingWindmill");


	//Material Class µî·Ï
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

	//Static_Mesh Load
	AssetLoader->LoadStaticMesh("Terrain_StaticMesh", "Terrain_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_StaticMesh", "ToonLink_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLinkTriangle_StaticMesh", "ToonLinkTriangle_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Tree_StaticMesh", "Tree_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Windmill_Body_StaticMesh", "Windmill_Body_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Windmill_Wing_StaticMesh", "Windmill_Wing_StaticMesh.json", AssetOwner::User);
}

std::string ReturnStartLevel()
{
	return "TriWorld";
}