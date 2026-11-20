#include "AWater.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPPlanarReflectionComponent.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include "ForestPearlEngine/FPAssetManager.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include <algorithm>
#include <cmath>

void AWater::Initialize()
{
	//Water surface (Plane_StaticMesh already lies flat, facing up, with no rotation)
	Surface = new FPStaticMeshComponent(this, "Plane_StaticMesh");
	Surface->SetupAttachment(RootComponent);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	Surface->SetMaterial(AssetManager->MakeMaterial("Water_Material"));

	//Component that computes the reflection plane/matrix. Its Forward (local +Z) is
	//the plane normal, so rotate -90 degrees to align it with the water surface
	//(local +Y, up); the visual Surface itself is left unrotated.
	Reflection = new FPPlanarReflectionComponent(this);
	Reflection->SetupAttachment(RootComponent);
	Reflection->SetRelativeRotation({ -90.0f, 0.0f, 0.0f });

	std::string WaterRenderTarget = "Water_RenderTarget";
	std::string WaterDepthStencil = "Water_DepthStencil";
	Reflection->SetRenderTarget(WaterRenderTarget);
	Reflection->SetDepthStencil(WaterDepthStencil);

	//Exclude the water plane itself from its own reflection render list
	Reflection->AddHiddenActor(this);

	ApplyWaterParameters();
}

void AWater::BeginPlay()
{
}

void AWater::ApplyWaterParameters()
{
	FPMaterialInterface& Material = Surface->GetMaterial();
	Material.SetParameter(FPShaderType::PS, "WaterSettings", "FlowOffset", static_cast<float>(FlowPhase));
	Material.SetParameter(FPShaderType::PS, "WaterSettings", "DistortionStrength", (std::max)(0.0f, Settings.DistortionStrength));
	Material.SetParameter(FPShaderType::PS, "WaterSettings", "Tiling", (std::max)(0.01f, Settings.Tiling));
	Material.SetParameter(FPShaderType::PS, "WaterSettings", "ReflectionAmount", std::clamp(Settings.ReflectionAmount, 0.0f, 1.0f));
	Material.SetParameter(FPShaderType::PS, "WaterSettings", "WaterColor", Settings.WaterColor);
}

void AWater::Tick()
{
	const float DeltaTime = GetWorld()->GetGameTimer()->DeltaTime();

	//Accumulate the flow UV phase, keeping it in [0,1) so a speed change never resets it
	const double Next = FlowPhase + static_cast<double>(Settings.FlowSpeed) * DeltaTime;
	FlowPhase = Next - std::floor(Next);

	ApplyWaterParameters();

	FPActor::Tick();
}
