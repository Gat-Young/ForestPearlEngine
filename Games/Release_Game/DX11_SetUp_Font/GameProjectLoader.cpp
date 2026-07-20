#include "ForestPearlEngine/GameProjectLoader.h"
#include "GameWorld.h"
#include "GameLevel.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"

void LoadClassRegist()
{
	GameProjectClassRegistry::Get().Register<GameWorld>("GameWorld");
	GameProjectClassRegistry::Get().Register<GameLevel>("GameLevel");
	GameProjectClassRegistry::Get().Register<GameMode>("GameMode");
	GameProjectClassRegistry::Get().Register<GameController>("GameController");
	GameProjectClassRegistry::Get().Register<UI>("UI");
}

std::string ReturnStartWorld()
{
	return "GameWorld";
}