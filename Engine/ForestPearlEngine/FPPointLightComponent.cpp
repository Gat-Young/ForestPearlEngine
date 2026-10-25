#include "FPPointLightComponent.h"

FPPointLightComponent::FPPointLightComponent(FPActor* Owner) : FPLocalLightComponent(Owner)
{
	FPLightConstantBuffer PointLightSetting;
	PointLightSetting.Direction = { 0.0f, 1.0f, 0.0f };
	PointLightSetting.Range = 1000.0f;
	PointLightSetting.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	PointLightSetting.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	PointLightSetting.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	PointLightSetting.Attenuation = { 1.0f, 1.0f, 1.0f, 1.0f };
	PointLightSetting.LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	PointLightSetting.LightType = LightType_Point;

	LightConstantBuffer = PointLightSetting;

	//상수 버퍼를 정점 상수 버퍼에 등록
	FPConstantBufferRenderDataUtil::AddConstantBuffer(VertexConstantBuffer, 0, LightConstantBuffer);
	RegistLightRenderList();
}

FPPointLightComponent::~FPPointLightComponent()
{
}
