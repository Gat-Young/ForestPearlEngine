#include "ADirectionalLight.h"
#include "ForestPearlEngine/FPDirectionalLightComponent.h"

ADirectionalLight::ADirectionalLight()
{
	LightComponent = new FPDirectionalLightComponent(this);
	FPVector3 Dir{ 0.0f, -2.0f, 3.0f };
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
