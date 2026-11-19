#pragma once
#include "ForestPearlEngine/FPSceneCaptureComponent2D.h"

class FPCameraComponent;

// Game-side specialization only. Uses the existing SceneCaptureRenderPass unchanged.
class WaterReflectionCaptureComponent final : public FPSceneCaptureComponent2D
{
public:
    explicit WaterReflectionCaptureComponent(FPActor* owner);
    void SetSourceCamera(FPCameraComponent* camera) { SourceCamera = camera; }
    void SetWaterHeight(float height) { WaterHeight = height; }
    void Tick() override;

private:
    FPCameraComponent* SourceCamera = nullptr;
    float WaterHeight = 0;
};
