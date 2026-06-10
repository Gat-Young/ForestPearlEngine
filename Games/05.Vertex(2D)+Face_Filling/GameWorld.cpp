#include "GameWorld.h"
#include "../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "UI.h"
#include "Triangle.h"

void GameWorld::Initialize()
{
	//World에서 사용되는 레벨 class 등록

	LevelList.push_back("GameLevel");
	__super::Initialize();
}

void GameWorld::BeginPlay()
{
	//UI* ui = new UI();
	//ForestPearlEngine::GetGameEngine().AddObjectTable(ui);
	//ForestPearlEngine::GetGameEngine().AddUITable(ui);

	//Triangle* tri = new Triangle();
	//ForestPearlEngine::GetGameEngine().AddObjectTable(tri);
	//ForestPearlEngine::GetGameEngine().AddRenderTable(tri);

	__super::BeginPlay();

}

void GameWorld::Tick()
{
	///게임 실행 부분
	__super::Tick();
}

