#include "GameDataLoader.h"
#include "../../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "../../../Games/BaseGame/Assets/Scripts/BaseGameManager.h"
#include "../../../Games/DOHWA(Interface+GUID+Query)/ADOHWAGameManager.h"

void GameDataLoader::LoadGame()
{
	ADOHWAGameManager* gm = new ADOHWAGameManager();
	ForestPearlEngine::GetGameEngine().AddObjectTable(gm);
}
