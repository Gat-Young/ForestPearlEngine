#include "../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "GameLoader/GameDataLoader.h"
#include <exception>

#include "Runner.h"

//Engine의 게임 실행 진입점
void Runner::Run()
{
    ForestPearlEngine& FPEngine = ForestPearlEngine::GetGameEngine();

    bool bIsSuccess;
    bIsSuccess = FPEngine.Initialize();
    if (!bIsSuccess)
        return;

    GameDataLoader DataLoader;
    DataLoader.LoadGame();

    FPEngine.GameLoop();

    FPEngine.Finalize();
}
