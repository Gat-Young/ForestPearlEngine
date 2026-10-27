#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPPointLightComponent;
class FPStaticMeshComponent;

class ATorchLight : public FPActor
{
private:
	FPPointLightComponent* LightComponent;
	FPStaticMeshComponent* LightProbe;

	bool isActive = false;

public:
	ATorchLight();
	~ATorchLight();

	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void OnTorch();
};