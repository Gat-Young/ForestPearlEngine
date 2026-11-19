#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPStaticMeshComponent;

class WaterDemoModels final : public FPActor
{
public:
    void Initialize() override;
    void BeginPlay() override {}
    void Tick() override;
private:
    FPStaticMeshComponent* RotatingModel = nullptr;
    float Angle = 0;
};
