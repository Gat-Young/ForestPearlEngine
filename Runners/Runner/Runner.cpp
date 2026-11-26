#include "../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include <exception>

#include "../../Engine/ForestPearlEngine/Utility/FPSystemTimer.h"
#include "Runner.h"

//Engine의 게임 실행 진입점
void Runner::Run()
{
    FPSystemTimer::Get();

    ForestPearlEngine& FPEngine = ForestPearlEngine::GetGameEngine();

    bool bIsSuccess;

    unsigned int profileID1 = FPSystemTimer::Get().StartProfileTimer();
    bIsSuccess = FPEngine.PreInitialize();

    FPSystemTimer::Get().EndProfileTimer("[TEST Log] 엔진 PreInitializer Time : ", profileID1);

    if (!bIsSuccess)
        return;

    bIsSuccess = FPEngine.Initialize();
    if (!bIsSuccess)
        return;

    FPEngine.GameLoop();

    FPEngine.Finalize();
}
