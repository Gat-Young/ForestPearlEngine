#include "../../Engine/ForestPearlEngine/GameProjectLoader.h"
#include "GameManager.h"
#include "Triangle.h"
#include "UI.h"

void LoadClassRegist()
{
	GameProjectClassRegistry::Get().Register<AGameManager>("AGameManager");
	GameProjectClassRegistry::Get().Register<UI>("UI");
	GameProjectClassRegistry::Get().Register<Triangle>("Triangle");
}

std::string ReturnStartWorld()
{
	return "AGameManager";
}