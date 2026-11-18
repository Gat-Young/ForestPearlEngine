#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPStaticMeshComponent;

class AWall : public FPActor
{
private:
	FPStaticMeshComponent* WallMesh;

public:
	AWall() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
};
