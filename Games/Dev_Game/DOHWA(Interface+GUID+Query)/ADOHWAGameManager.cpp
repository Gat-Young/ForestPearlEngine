#include "ADOHWAGameManager.h"
#include "../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "UI.h"

void ADOHWAGameManager::BeginPlay()
{
	UI* ui = new UI();
	ForestPearlEngine::GetGameEngine().AddObjectTable(ui);
	ForestPearlEngine::GetGameEngine().AddUITable(ui);
}

void ADOHWAGameManager::Tick()
{
	///게임 실행 부분
}
