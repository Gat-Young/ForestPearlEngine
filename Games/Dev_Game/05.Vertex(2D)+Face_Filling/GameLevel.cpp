#include "GameLevel.h"
#include "Triangle.h"
#include "UI.h"

void GameLevel::Initialize()
{
	ActorlList.push_back("Triangle");
	ActorlList.push_back("UI");
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