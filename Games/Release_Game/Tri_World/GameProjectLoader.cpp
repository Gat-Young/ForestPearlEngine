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

void RegistProjectName()
{
	FPPathManager::Get().Initialize("Tri_World");
}

void LoadLevel()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	AssetManager->LoadLevelData("TriWorld", "TriWorld.json");

}

void LoadClassRegist()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<GameMode>("GameMode");
	ClassRegistry->Register<GameController>("GameController");
	ClassRegistry->Register<UI>("UI");
	ClassRegistry->Register<Player>("Player");
	ClassRegistry->Register<Tree>("Tree");
	ClassRegistry->Register<GameCamera>("GameCamera");
	ClassRegistry->Register<Grid>("Grid");
	ClassRegistry->Register<Axis>("Axis");
	ClassRegistry->Register<Terrain>("Terrain");
	ClassRegistry->Register<Windmill>("Windmill");
}

void LoadAssets()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	AssetManager->LoadFbxData("ToonLink/ToonLinkTriangle.fbx");
	AssetManager->LoadFbxData("ToonLink/ToonLink.fbx");
	AssetManager->LoadFbxData("Terrain/Terrain.fbx");
	AssetManager->LoadFbxData("Tree/Tree.fbx");
	AssetManager->LoadFbxData("Windmill/Windmill_Body.fbx");
	AssetManager->LoadFbxData("Windmill/Windmill_Wing.fbx");
}

std::string ReturnStartLevel()
{
	return "TriWorld";
}