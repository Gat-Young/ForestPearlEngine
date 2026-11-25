#include "FPDirectionalLightComponent.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"

#include <iostream>

FPDirectionalLightComponent::FPDirectionalLightComponent(FPActor* Owner) : FPLightComponent(Owner)
{
	FPLightConstantBuffer DirectionLightSetting;
	DirectionLightSetting.Direction = { 0.0f, 1.0f, 0.0f };
	DirectionLightSetting.Range = 1000.0f;
	DirectionLightSetting.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectionLightSetting.Ambient = { 0.35f, 0.35f, 0.35f, 1.0f };
	DirectionLightSetting.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectionLightSetting.Attenuation = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectionLightSetting.LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectionLightSetting.LightType = LightType_Direction;

	LightConstantBuffer = DirectionLightSetting;

	RegistLightRenderList();

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	CascadeShadowMapName = AssetManager->CreateCascadeShadowMap(CascadeCount, ShadowResolution);
}

void FPDirectionalLightComponent::SetCascadeCount(unsigned int InCascadeCount)
{
	CascadeCount = InCascadeCount;

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	AssetManager->UpdateCascadeShadowMap(CascadeShadowMapName, CascadeCount, ShadowResolution);
}

void FPDirectionalLightComponent::SetShadowResolution(unsigned int InShadowResolution)
{
	ShadowResolution = InShadowResolution;

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	AssetManager->UpdateCascadeShadowMap(CascadeShadowMapName, CascadeCount, ShadowResolution);
}

FPDirectionalLightComponent::~FPDirectionalLightComponent()
{
}
