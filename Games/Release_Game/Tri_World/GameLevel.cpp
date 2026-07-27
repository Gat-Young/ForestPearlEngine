#include "GameLevel.h"

void GameLevel::Initialize()
{
	//{Class name, Instance Name}
	ActorList.push_back({ "GameCamera", "MainCamera"});
	ActorList.push_back({ "UI", "System UI" });
	ActorList.push_back({ "Player", "Link" });
	ActorList.push_back({ "Grid", "Grid" });
	ActorList.push_back({ "Axis", "Axis" });
	ActorList.push_back({ "Terrain", "Ground" });
	ActorList.push_back({ "Tree", "Tree1" });
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