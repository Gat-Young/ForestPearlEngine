#include "WaterReflectionCaptureComponent.h"
#include "WaterMath.h"
#include "ForestPearlEngine/FPCameraComponent.h"

WaterReflectionCaptureComponent::WaterReflectionCaptureComponent(FPActor* owner)
    : FPSceneCaptureComponent2D(owner)
{
}

void WaterReflectionCaptureComponent::Tick()
{
    FPSceneCaptureComponent2D::Tick();
    if (!SourceCamera || !SourceCamera->Active)
    {
        SetActive(false);
        return;
    }
    SetActive(true);
    ViewMatrix = FPMatrix(WaterMath::ReflectView(SourceCamera->ViewMatrix.Matrix, WaterHeight));
    ProjectionMatrix = SourceCamera->ProjectionMatrix;
    FPVector3 eye = SourceCamera->GetComponentLocation();
    eye.y = 2.0f * WaterHeight - eye.y;
    WorldTransform.Location = eye;
    WorldTransform.LocationMatrix = FPMatrix(DirectX::XMMatrixTranslation(eye.x, eye.y, eye.z));
}
