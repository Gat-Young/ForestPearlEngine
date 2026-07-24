#include "ForestPearlEngine/GameProjectLoader.h"
#include "ForestPearlEngine/AssetManager.h"
#include "GameWorld.h"
#include "GameLevel.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"
#include "Player.h"
#include "GameCamera.h"
#include "Grid.h"
#include "Axis.h"
#include "Terrain.h"

void LoadClassRegist()
{
	GameProjectClassRegistry::Get().Register<GameWorld>("GameWorld");
	GameProjectClassRegistry::Get().Register<GameLevel>("GameLevel");
	GameProjectClassRegistry::Get().Register<GameMode>("GameMode");
	GameProjectClassRegistry::Get().Register<GameController>("GameController");
	GameProjectClassRegistry::Get().Register<UI>("UI");
	GameProjectClassRegistry::Get().Register<Player>("Player");
	GameProjectClassRegistry::Get().Register<GameCamera>("GameCamera");
	GameProjectClassRegistry::Get().Register<Grid>("Grid");
	GameProjectClassRegistry::Get().Register<Axis>("Axis");
	GameProjectClassRegistry::Get().Register<Terrain>("Terrain");
}

void LoadAssets()
{
	AssetManager::Get().LoadFbxData("Model/ToonLink/ToonLinkTriangle.fbx");
	AssetManager::Get().LoadFbxData("Model/ToonLink/ToonLink.fbx");
	AssetManager::Get().LoadFbxData("Model/Terrain/Terrain.fbx");
}

std::string ReturnStartWorld()
{
	return "GameWorld";
}