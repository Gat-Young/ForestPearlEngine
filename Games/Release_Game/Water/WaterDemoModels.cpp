#include "WaterDemoModels.h"
#include "WaterAssets.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/FPGameTimer.h"

void WaterDemoModels::Initialize()
{
    WaterAssets::LoadShadersAndBump();
    auto addModel = [this](const char* mesh, FPVector3 position, FPVector3 scale, FPVector4 color)
    {
        auto* model = new FPStaticMeshComponent(this, mesh);
        model->SetupAttachment(RootComponent);
        model->SetRelativeLocation(position);
        model->SetRelativeScale3D(scale);
        model->GetMaterial().SetParameter(FPShaderType::PS, "ModelSettings", "Tint", color);
        return model;
    };
    addModel("WaterCube", {-4, 1.7f, 2}, {2.4f, 3.4f, 2.4f}, {0.95f, 0.35f, 0.12f, 1});
    addModel("WaterCube", {3, 2.2f, 3}, {1.0f, 4.4f, 1.0f}, {0.12f, 0.65f, 0.85f, 1});
    addModel("WaterCube", {6, 2.2f, 3}, {1.0f, 4.4f, 1.0f}, {0.12f, 0.65f, 0.85f, 1});
    addModel("WaterCube", {4.5f, 4.7f, 3}, {4.0f, 0.8f, 1.2f}, {0.92f, 0.8f, 0.35f, 1});
    RotatingModel = addModel("WaterCube", {0, 2.0f, -1.5f}, {2.0f, 2.0f, 2.0f}, {0.72f, 0.32f, 0.85f, 1});
}

void WaterDemoModels::Tick()
{
    Angle += GetWorld()->GetGameTimer()->DeltaTime() * 20.0f;
    if (RotatingModel) RotatingModel->SetRelativeRotation({15.0f, Angle, 10.0f});
    FPActor::Tick();
}
