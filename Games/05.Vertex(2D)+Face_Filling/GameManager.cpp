#include "GameManager.h"
#include "../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "UI.h"
#include "Triangle.h"

void AGameManager::BeginPlay()
{
	UI* ui = new UI();
	ForestPearlEngine::GetGameEngine().AddObjectTable(ui);
	ForestPearlEngine::GetGameEngine().AddUITable(ui);

	Triangle* tri = new Triangle();
	ForestPearlEngine::GetGameEngine().AddObjectTable(tri);
	ForestPearlEngine::GetGameEngine().AddRenderTable(tri);

}

void AGameManager::Tick()
{
	///게임 실행 부분
}

