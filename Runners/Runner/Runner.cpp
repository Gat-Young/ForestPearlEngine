#include "../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include <exception>

#include "Runner.h"

//Engine의 게임 실행 진입점
void Runner::Run()
{
    ForestPearlEngine& FPEngine = ForestPearlEngine::GetGameEngine();

    bool bIsSuccess;

    bIsSuccess = FPEngine.PreInitialize();
    if (!bIsSuccess)
        return;

    bIsSuccess = FPEngine.Initialize();
    if (!bIsSuccess)
        return;

    FPEngine.GameLoop();

    FPEngine.Finalize();
}
