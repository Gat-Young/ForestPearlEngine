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
	FPVector3 ProbeSize = { 0.5f, 0.5f, 0.5f };
	LightComponent->SetLightColor(Color);
	LightProbe->SetWorldScale3D({ 0.5f, 0.5f, 0.5f });
	LightProbe->GetMaterial().SetBaseColor(Color);
	LightProbe->GetMaterial().SetTwoSided(true);

	FPVector4 EmissiveColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	LightProbe->GetMaterial().SetEmissive(EmissiveColor);
	FPVector4 Attenuation = { 0.0f, 0.05f, 0.05f , 1.0f };
	LightComponent->SetAttenuation(Attenuation);

	LightComponent->SetRange(100.0f);

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
