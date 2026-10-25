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

void ADirectionalLight::SetDirectionalLightColor()
{
	NowLight++;
	NowLight %= MaxLight;

	FPVector4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (NowLight == 0) { LightColor = { 1.0f, 1.0f, 1.0f, 1.0f }; }
	if (NowLight == 1) { LightColor = { 0.901961f, 0.501961f, 0.101961f, 1.0f }; }
	if (NowLight == 2) { LightColor = { 0.0f, 0.0f, 0.301961f, 1.0f }; }
	if (NowLight == 3) { LightColor = { 0.301961f, 0.301961f, 0.701961f, 1.0f }; }

	LightComponent->SetLightColor(LightColor);
}
