#include "GameDataLoader.h"
#include "../../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "../../../Games/BaseGame/Assets/Scripts/BaseGameManager.h"

void GameDataLoader::LoadGame()
{
	ABaseGameManager* gm = new ABaseGameManager();
	ForestPearlEngine::GetGameEngine().AddObjectTable(gm);
}
