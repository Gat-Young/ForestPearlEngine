#include "GameWorld.h"

void GameWorld::Initialize()
{
	//World에서 사용되는 GameMode class 등록
	WorldSetting.GameMode = "GameMode";

	//World에서 사용되는 레벨 class 등록
	WorldSetting.LevelList.push_back("GameLevel");
	__super::Initialize();
}

void GameWorld::BeginPlay()
{

	__super::BeginPlay();

}

void GameWorld::Tick()
{
	///게임 실행 부분
	__super::Tick();
}

