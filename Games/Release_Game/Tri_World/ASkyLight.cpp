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

std::string ASkyLight::ToggleEnvironmentMap()
{
	bUseAlternateEnvironmentMap = !bUseAlternateEnvironmentMap;

	std::string EnvironmentMap = bUseAlternateEnvironmentMap ? "uffizi_cross_Mark_dds" : "rustig_koppie_puresky_2k";
	SkyLightComponent->SetCubeMap(EnvironmentMap);

	return EnvironmentMap;
}
