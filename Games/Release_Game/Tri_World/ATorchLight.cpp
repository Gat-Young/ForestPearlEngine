#include "ATorchLight.h"
#include "ForestPearlEngine/FPPointLightComponent.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include <iostream>

ATorchLight::ATorchLight()
{
	LightComponent = new FPPointLightComponent(this);
	LightComponent->SetupAttachment(RootComponent);
	LightProbe = new FPStaticMeshComponent(this, "Sphere_StaticMesh");
	LightProbe->SetupAttachment(RootComponent);
	FPVector4 Color = { 1, 0.5f, 0.f, 1 };
	LightComponent->SetLightColor(Color);
	LightProbe->GetMaterial().SetBaseColor(Color);

	FPVector4 Attenuation = { 0, 0.1f, 0.1f , 1.0f };
	LightComponent->SetAttenuation(Attenuation);

	LightComponent->SetActive(isActive);
	LightProbe->SetActive(isActive);
}

ATorchLight::~ATorchLight()
{
}

void ATorchLight::Initialize()
{
}

void ATorchLight::BeginPlay()
{
}

void ATorchLight::Tick()
{
	__super::Tick();
}

void ATorchLight::OnTorch()
{
	isActive = !isActive;
	LightComponent->SetActive(isActive);
	LightProbe->SetActive(isActive);
}
