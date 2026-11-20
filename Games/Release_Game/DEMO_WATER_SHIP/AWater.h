#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPStaticMeshComponent;
class FPPlanarReflectionComponent;

//Ported from the AWater demo: builds a flowing water surface through the JSON Load
//structure (Water_Material.json), using the engine's FPPlanarReflectionComponent for
//reflection instead of the demo's own WaterReflectionCaptureComponent.
struct WaterSettings
{
	float FlowSpeed = 0.3f;           //UV flow speed (U direction, per second)
	float DistortionStrength = 0.8f;  //Bump height-gradient distortion strength
	float Tiling = 2.0f;              //Bump map UV tiling
	float ReflectionAmount = 0.8f;    //Reflection blend amount
	FPVector4 WaterColor = { 0.035f, 0.18f, 0.24f, 1.0f };
};

class AWater final : public FPActor
{
public:
	WaterSettings Settings;

	AWater() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

private:
	void ApplyWaterParameters();

	FPStaticMeshComponent* Surface = nullptr;
	FPPlanarReflectionComponent* Reflection = nullptr;
	double FlowPhase = 0.0;
};
