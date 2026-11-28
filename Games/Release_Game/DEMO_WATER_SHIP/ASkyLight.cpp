#include "ASkyLight.h"
#include "ForestPearlEngine/FPSkyLightComponent.h"

ASkyLight::ASkyLight()
{
	SkyLightComponent = new FPSkyLightComponent(this);
	SkyLightComponent->SetCubeMap("rustig_koppie_puresky_2k");
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
