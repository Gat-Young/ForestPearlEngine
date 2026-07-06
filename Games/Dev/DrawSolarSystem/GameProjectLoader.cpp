#include "../../Engine/ForestPearlEngine/GameProjectLoader.h"
#include "GameWorld.h"
#include "GameLevel.h"
#include "Orb.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"
#include "GameCamera.h"

void LoadClassRegist()
{
	GameProjectClassRegistry::Get().Register<GameCamera>("GameCamera");
	GameProjectClassRegistry::Get().Register<GameWorld>("GameWorld");
	GameProjectClassRegistry::Get().Register<GameLevel>("GameLevel");
	GameProjectClassRegistry::Get().Register<GameMode>("GameMode");
	GameProjectClassRegistry::Get().Register<GameController>("GameController");
	GameProjectClassRegistry::Get().Register<UI>("UI");
	GameProjectClassRegistry::Get().Register<FPOrb>("FPOrb");
}

std::string ReturnStartWorld()
{
	return "GameWorld";
}