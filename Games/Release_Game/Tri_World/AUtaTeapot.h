#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPStaticMeshComponent;

class AUtaTeapot : public FPActor
{
private:
	FPStaticMeshComponent* TeapotUp;
	FPStaticMeshComponent* TeapotBottom;

public:
	AUtaTeapot() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
};
