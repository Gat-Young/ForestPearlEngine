#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPDirectionalLightComponent;

class ADirectionalLight : public FPActor
{
	private:
		FPDirectionalLightComponent* LightComponent;

	public:
		ADirectionalLight();
		~ADirectionalLight();

		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
};