#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPStaticMeshComponent;
class FPPlanarReflectionComponent;

class ALake : public FPActor
{
private:
	FPStaticMeshComponent* LakeMesh;
	FPPlanarReflectionComponent* ReflectionComponent;

public:
	ALake() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
};
