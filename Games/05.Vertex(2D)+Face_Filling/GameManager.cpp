#include "GameManager.h"
#include "../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "UI.h"

void AGameManager::BeginPlay()
{
	UI* ui = new UI();
	ForestPearlEngine::GetGameEngine().AddObjectTable(ui);
	ForestPearlEngine::GetGameEngine().AddUITable(ui);
}

void AGameManager::Tick()
{
	///게임 실행 부분
}

