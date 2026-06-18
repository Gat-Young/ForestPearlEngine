#include "../../Engine/ForestPearlEngine/GameProjectLoader.h"
#include "GameWorld.h"
#include "GameLevel.h"
#include "Triangle.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"
#include "GameCamera.h"
#include "Grid.h"

void LoadClassRegist()
{
	GameProjectClassRegistry::Get().Register<GameWorld>("GameWorld");
	GameProjectClassRegistry::Get().Register<GameLevel>("GameLevel");
	GameProjectClassRegistry::Get().Register<GameMode>("GameMode");
	GameProjectClassRegistry::Get().Register<GameController>("GameController");
	GameProjectClassRegistry::Get().Register<UI>("UI");
	GameProjectClassRegistry::Get().Register<Grid>("Grid");
	GameProjectClassRegistry::Get().Register<Triangle>("Triangle");
	GameProjectClassRegistry::Get().Register<GameCamera>("GameCamera");

}

std::string ReturnStartWorld()
{
	return "GameWorld";
}