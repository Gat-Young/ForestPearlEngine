#include "GameDataLoader.h"
#include "../../../Engine/ForestPearlEngine/ForestPearlEngine.h"
//#include "../../../Games/BaseGame/Assets/Scripts/BaseGameManager.h"
//#include "../../../Games/DOHWA(Interface+GUID+Query)/ADOHWAGameManager.h"
#include "../../../Games/05.Vertex(2D)+Face_Filling/GameManager.h"

void GameDataLoader::LoadGame()
{
	AGameManager* gm = new AGameManager();
	ForestPearlEngine::GetGameEngine().AddObjectTable(gm);
}
