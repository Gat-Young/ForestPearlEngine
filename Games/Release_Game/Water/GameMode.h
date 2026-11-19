#pragma once
#include "ForestPearlEngine/FPAGameMode.h"

class GameMode final : public FPAGameMode
{
public:
    void Initialize() override
    {
        // The existing controller consumes queued raw-input events; AWater polls key states.
        ControllerList = {"WaterGameController"};
        FPAGameMode::Initialize();
    }
    void BeginPlay() override { FPAGameMode::BeginPlay(); }
    void Tick() override { FPAGameMode::Tick(); }
};
