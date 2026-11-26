#include "FPDirectionalLightComponent.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include "FPCascadeShadowRenderList.h"

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

	RegistCascadeShadowRenderList();
}

void FPDirectionalLightComponent::RegistCascadeShadowRenderList()
{
	//Cascade Shadow Render List에 등록
	FPCascadeShadowRenderList* CascadeShadowRenderList = static_cast<FPCascadeShadowRenderList*>(FPGameInstance::Get().GetCascadeShadowRenderList());
	CascadeShadowRenderItem = CascadeShadowRenderList->RegistRenderList();

	if (CascadeShadowRenderItem == nullptr) { std::cout << "[DirectionalLightComponent] : 등록 받은 Cascade Shadow 렌더 아이템이 없습니다." << "\n"; return; }

	CascadeShadowRenderItem->CastShadow = &(this->CastShadow);
	CascadeShadowRenderItem->Direction = &(this->LightConstantBuffer.Direction);
	CascadeShadowRenderItem->CascadeCount = &(this->CascadeCount);
	CascadeShadowRenderItem->ShadowResolution = &(this->ShadowResolution);
	CascadeShadowRenderItem->DynamicShadowDistance = &(this->DynamicShadowDistance);
	CascadeShadowRenderItem->CascadeSplitLambda = &(this->CascadeSplitLambda);
	CascadeShadowRenderItem->CascadeTransitionFraction = &(this->CascadeTransitionFraction);
	CascadeShadowRenderItem->ShadowDistanceFadeoutFraction = &(this->ShadowDistanceFadeoutFraction);
	CascadeShadowRenderItem->ShadowBias = &(this->ShadowBias);
	CascadeShadowRenderItem->ShadowSlopeBias = &(this->ShadowSlopeBias);
	CascadeShadowRenderItem->DistanceMargin = &(this->DistanceMargin);
	CascadeShadowRenderItem->CascadeShadowMapName = &(this->CascadeShadowMapName);
}

void FPDirectionalLightComponent::SetCascadeCount(unsigned int InCascadeCount)
{
	//Cascade Shadow Map(Texture2DArray)의 최대 슬라이스 개수(MAX_CASCADE, CascadeShadow.hlsli)에 맞춰 Clamp
	if (InCascadeCount > 8) InCascadeCount = 8;

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

void FPDirectionalLightComponent::SetDynamicShadowDistance(float InDynamicShadowDistance)
{
	DynamicShadowDistance = InDynamicShadowDistance;
}

void FPDirectionalLightComponent::SetCascadeSplitLambda(float InCascadeSplitLambda)
{
	CascadeSplitLambda = InCascadeSplitLambda;
}

void FPDirectionalLightComponent::SetCascadeTransitionFraction(float InCascadeTransitionFraction)
{
	CascadeTransitionFraction = InCascadeTransitionFraction;
}

void FPDirectionalLightComponent::SetShadowDistanceFadeoutFraction(float InShadowDistanceFadeoutFraction)
{
	ShadowDistanceFadeoutFraction = InShadowDistanceFadeoutFraction;
}

void FPDirectionalLightComponent::SetShadowBias(float InShadowBias)
{
	ShadowBias = InShadowBias;
}

void FPDirectionalLightComponent::SetShadowSlopeBias(float InShadowSlopeBias)
{
	ShadowSlopeBias = InShadowSlopeBias;
}

void FPDirectionalLightComponent::SetDistanceMargin(float InDistanceMargin)
{
	DistanceMargin = InDistanceMargin;
}

FPDirectionalLightComponent::~FPDirectionalLightComponent()
{
}
