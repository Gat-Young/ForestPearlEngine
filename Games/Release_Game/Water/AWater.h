#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "WaterSettings.h"

class FPCameraComponent;
class FPStaticMeshComponent;
class FPSkyLightComponent;
class WaterReflectionCaptureComponent;

class AWater final : public FPActor
{
public:
    WaterSettings Settings;
    void Initialize() override;
    void BeginPlay() override;
    void Tick() override;

private:
    void UpdateInput(float deltaTime);
    void ApplyWaterParameters();
    FPCameraComponent* Camera = nullptr;
    FPStaticMeshComponent* Surface = nullptr;
    FPSkyLightComponent* SkyLight = nullptr;
    WaterReflectionCaptureComponent* Reflection = nullptr;
    double FlowPhase = 0;
    float CameraYaw = -18.0f;
    float CameraPitch = 24.0f;
    float CameraDistance = 24.0f;
};
