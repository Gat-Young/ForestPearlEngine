#include "AWater.h"
#include "WaterAssets.h"
#include "WaterMath.h"
#include "WaterReflectionCaptureComponent.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPSkyLightComponent.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/Systems/InputSystem.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

void AWater::Initialize()
{
    WaterAssets::LoadShadersAndBump();
    Surface = new FPStaticMeshComponent(this, "WaterPlane");
    Surface->SetupAttachment(RootComponent);

    // The engine already loads this cubemap and binds the first SkyLight at t127/s15/b1.
    SkyLight = new FPSkyLightComponent(this);
    SkyLight->SetupAttachment(RootComponent);
    SkyLight->SetCubeMap("rustig_koppie_puresky_2k");

    // Siblings are ticked in attachment order: camera first, reflection capture second.
    Camera = new FPCameraComponent(this);
    Camera->SetupAttachment(RootComponent);
    Camera->Zn = 0.1f;
    Camera->Zf = 150.0f;
    Reflection = new WaterReflectionCaptureComponent(this);
    Reflection->SetupAttachment(RootComponent);
    Reflection->SetSourceCamera(Camera);
    std::string target = "WaterReflection";
    std::string depth = "WaterReflectionDepth";
    Reflection->SetRenderTarget(target);
    Reflection->SetDepthStencil(depth);
    Reflection->AddHiddenActor(this);
    ApplyWaterParameters();
}

void AWater::BeginPlay()
{
    std::cout << "Water: arrows orbit | W/S zoom | 1/2 flow speed | 3/4 distortion\n"
        "Edit WaterSettings.h for water color, tiling and reflection amount.\n";
}

void AWater::UpdateInput(float deltaTime)
{
    auto* input = static_cast<FPInputSystem*>(FPGameInstance::Get().GetInputSystem());
    const auto axis = [input](USHORT positive, USHORT negative)
    {
        return static_cast<float>(input->bIsKeyDown(positive)) - static_cast<float>(input->bIsKeyDown(negative));
    };
    CameraYaw += axis(VK_RIGHT, VK_LEFT) * 35.0f * deltaTime;
    CameraPitch = std::clamp(CameraPitch + axis(VK_DOWN, VK_UP) * 25.0f * deltaTime, 8.0f, 65.0f);
    CameraDistance = std::clamp(CameraDistance + axis('S', 'W') * 10.0f * deltaTime, 10.0f, 45.0f);
    Settings.FlowSpeed = std::clamp(Settings.FlowSpeed + axis('2', '1') * 0.2f * deltaTime, 0.0f, 2.0f);
    Settings.DistortionStrength = std::clamp(Settings.DistortionStrength + axis('4', '3') * deltaTime, 0.0f, 4.0f);
}

void AWater::ApplyWaterParameters()
{
    auto& material = Surface->GetMaterial();
    const bool valid = material.SetParameter(FPShaderType::PS, "WaterSettings", "FlowOffset", static_cast<float>(FlowPhase))
        && material.SetParameter(FPShaderType::PS, "WaterSettings", "DistortionStrength", (std::max)(0.0f, Settings.DistortionStrength))
        && material.SetParameter(FPShaderType::PS, "WaterSettings", "Tiling", (std::max)(0.01f, Settings.Tiling))
        && material.SetParameter(FPShaderType::PS, "WaterSettings", "ReflectionAmount", std::clamp(Settings.ReflectionAmount, 0.0f, 1.0f))
        && material.SetParameter(FPShaderType::PS, "WaterSettings", "WaterColor", Settings.WaterColor)
        && material.SetParameter(FPShaderType::PS, "WaterSettings", "EnvironmentAmount", (std::max)(0.0f, Settings.EnvironmentAmount))
        && material.SetParameter(FPShaderType::PS, "WaterSettings", "EnvironmentDistortionScale", (std::max)(0.0f, Settings.EnvironmentDistortionScale));
    SkyLight->SetLightIntnesity((std::max)(0.0f, Settings.EnvironmentIntensity));
    if (!valid) throw std::runtime_error("Water material parameters do not match the shaders.");
}

void AWater::Tick()
{
    const float deltaTime = GetWorld()->GetGameTimer()->DeltaTime();
    UpdateInput(deltaTime);
    FlowPhase = WaterMath::AdvanceFlow(FlowPhase, Settings.FlowSpeed, deltaTime);
    ApplyWaterParameters();
    const float yaw = DegToRad(CameraYaw);
    const float pitch = DegToRad(CameraPitch);
    Camera->SetRelativeLocation({-std::sin(yaw) * std::cos(pitch) * CameraDistance,
        1.0f + std::sin(pitch) * CameraDistance, -std::cos(yaw) * std::cos(pitch) * CameraDistance});
    Camera->SetRelativeRotation({CameraPitch, CameraYaw, 0});
    Reflection->SetWaterHeight(GetActorLocation().y);
    FPActor::Tick();
}
