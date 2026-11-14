#include "ASkyLight.h"
#include "ForestPearlEngine/FPSkyLightComponent.h"

ASkyLight::ASkyLight()
{
	SkyLightComponent = new FPSkyLightComponent(this);
	SkyLightComponent->SetCubeMap("Cross_Roads_Probe_bmp");

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
