#include "ADirectionalLight.h"
#include "ForestPearlEngine/FPDirectionalLightComponent.h"

ADirectionalLight::ADirectionalLight()
{
	LightComponent = new FPDirectionalLightComponent(this);
	FPVector3 Dir{ 0.43f, -0.64f, 0.64f };
	LightComponent->SetDirection(Dir);

}

ADirectionalLight::~ADirectionalLight()
{
}

void ADirectionalLight::Initialize()
{
}

void ADirectionalLight::BeginPlay()
{
}

void ADirectionalLight::Tick()
{
	__super::Tick();
}
