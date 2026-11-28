#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPStaticMeshComponent;

class ASkyBox : public FPActor
{
private:
	FPStaticMeshComponent* SkyBoxMesh;

public:
	ASkyBox() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
};
