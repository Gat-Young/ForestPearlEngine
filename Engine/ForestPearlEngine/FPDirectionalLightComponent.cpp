#include "FPDirectionalLightComponent.h"

#include <iostream>

FPDirectionalLightComponent::FPDirectionalLightComponent(FPActor* Owner) : FPLightComponent(Owner)
{
	FPLightConstantBuffer DirectionLightSetting;
	DirectionLightSetting.Direction = { 0.0f, 1.0f, 0.0f };
	DirectionLightSetting.Range = 1000.0f;
	DirectionLightSetting.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectionLightSetting.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	DirectionLightSetting.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectionLightSetting.Attenuation = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectionLightSetting.LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectionLightSetting.LightType = LightType_Direction;

	LightConstantBuffer = DirectionLightSetting;

	//상수 버퍼를 정점 상수 버퍼에 등록
	RegistLightRenderList();
}

FPDirectionalLightComponent::~FPDirectionalLightComponent()
{
}
