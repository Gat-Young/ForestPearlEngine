#include "GameLevel.h"

void GameLevel::Initialize()
{
	//{Class name, Instance Name}
	ActorlList.push_back({ "GameCamera", "MainCamera"});
	ActorlList.push_back({ "UI", "System UI" });
	ActorlList.push_back({ "Triangle", "Triangle1" });
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