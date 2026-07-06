#include "BaseGameManager.h"
#include "../../../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "Player.h"

void ABaseGameManager::BeginPlay()
{
	Player* p = new Player();
	p->Transform = { 600.0f, 300.0f, 0.0f };
	ForestPearlEngine::GetGameEngine().AddObjectTable(p);
	ForestPearlEngine::GetGameEngine().AddRenderTable(p);
}

void ABaseGameManager::Tick()
{
	//// 게임 실행 부분
}
