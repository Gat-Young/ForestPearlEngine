#include "GameLevel.h"
#include "Triangle.h"
#include "UI.h"

void GameLevel::Initialize()
{
	ActorlList.push_back("GameCamera");
	ActorlList.push_back("Triangle");
	ActorlList.push_back("UI");
	ActorlList.push_back("Grid");
	__super::Initialize();
}

void GameLevel::BeginPlay()
{
	__super::BeginPlay();
}

void GameLevel::Tick()
{
	__super::Tick();
}