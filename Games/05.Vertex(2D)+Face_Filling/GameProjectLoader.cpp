#include "../../Engine/ForestPearlEngine/GameProjectLoader.h"
#include "GameWorld.h"
#include "GameLevel.h"
#include "Triangle.h"
#include "UI.h"

void LoadClassRegist()
{
	GameProjectClassRegistry::Get().Register<GameWorld>("GameWorld");
	GameProjectClassRegistry::Get().Register<GameLevel>("GameLevel");
	GameProjectClassRegistry::Get().Register<UI>("UI");
	GameProjectClassRegistry::Get().Register<Triangle>("Triangle");
}

std::string ReturnStartWorld()
{
	return "GameWorld";
}