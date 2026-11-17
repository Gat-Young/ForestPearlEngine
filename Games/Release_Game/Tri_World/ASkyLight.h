#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPSkyLightComponent;

class ASkyLight : public FPActor
{
	private:
		FPSkyLightComponent* SkyLightComponent;

		bool bUseAlternateEnvironmentMap = false;

	public:
		ASkyLight();
		~ASkyLight();

		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		std::string ToggleEnvironmentMap();
};
