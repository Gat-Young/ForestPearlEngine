#include "../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "GameLoader/GameDataLoader.h"
#include <exception>


//Engine의 게임 실행 진입점
int main()
{
    ForestPearlEngine& FPEngine = ForestPearlEngine::GetGameEngine();

    bool bIsSuccess;
    bIsSuccess = FPEngine.Initialize();
    if (!bIsSuccess)
        return 0;

    GameDataLoader DataLoader;
    DataLoader.LoadGame();

    FPEngine.GameLoop();

    FPEngine.Finalize();

    return 0;
}
