#include "GameLevel.h"

void GameLevel::Initialize()
{
	//{Class name, Instance Name}
	ActorlList.push_back({ "UI", "System UI" });
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