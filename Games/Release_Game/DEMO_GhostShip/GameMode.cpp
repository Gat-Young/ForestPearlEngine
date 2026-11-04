#include "GameMode.h"

void GameMode::Initialize()
{
	ControllerList.push_back("GameController");
	__super::Initialize();
}

void GameMode::BeginPlay()
{
	__super::BeginPlay();
}

void GameMode::Tick()
{
	__super::Tick();
}
