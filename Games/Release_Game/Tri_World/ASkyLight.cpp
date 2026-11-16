#include "ASkyLight.h"
#include "ForestPearlEngine/FPSkyLightComponent.h"

ASkyLight::ASkyLight()
{
	SkyLightComponent = new FPSkyLightComponent(this);
	SkyLightComponent->SetCubeMap("uffizi_cross_dds");
	SkyLightComponent->SetLightIntnesity(0.5f);
}

ASkyLight::~ASkyLight()
{
}

void ASkyLight::Initialize()
{
}

void ASkyLight::BeginPlay()
{
}

void ASkyLight::Tick()
{
	__super::Tick();
}
