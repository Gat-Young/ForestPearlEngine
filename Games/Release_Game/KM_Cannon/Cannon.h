#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPStaticMeshComponent;

class Cannon : public FPActor
{
private:
	FPStaticMeshComponent* Mesh;

public:
	virtual void Initialize() override;
	virtual void BeginPlay() override;
};
