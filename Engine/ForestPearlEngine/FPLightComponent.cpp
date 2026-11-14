#include "FPLightComponent.h"
#include "FPGameInstance.h"
#include <iostream>

FPLightComponent::FPLightComponent(FPActor* Owner) : FPLightComponentBase(Owner)
{
}

void FPLightComponent::RegistLightRenderList()
{
	//Light Render List에 등록
	FPLightRenderList* LightRenderList = static_cast<FPLightRenderList*>(FPGameInstance::Get().GetLightRenderList());
	RenderItem = LightRenderList->RegistRenderList();

	if (RenderItem == nullptr) { std::cout << "[DirectionalLightComponent] : 등록 받은 렌더 아이템이 없습니다." << "\n"; return; }

	RenderItem->LightType = &(this->LightConstantBuffer.LightType);
	RenderItem->Active = &(this->isActive);
	RenderItem->Range = &(this->LightConstantBuffer.Range);
	RenderItem->Direction = &(this->LightConstantBuffer.Direction);
	RenderItem->Diffuse = &(this->LightConstantBuffer.Diffuse);
	RenderItem->Ambient = &(this->LightConstantBuffer.Ambient);
	RenderItem->Specular = &(this->LightConstantBuffer.Specular);
	RenderItem->Attenuation = &(this->LightConstantBuffer.Attenuation);
	RenderItem->LightColor = &(this->LightConstantBuffer.LightColor);
	RenderItem->Location = &(this->WorldTransform.LocationMatrix);
	RenderItem->Rotation = &(this->WorldTransform.RotationMatrix);
	RenderItem->Scale = &(this->WorldTransform.ScaleMatrix);
}

//Set한 이후 버퍼 데이터를 업데이트
void FPLightComponent::SetDirection(FPVector3& Dir)
{
	LightConstantBuffer.Direction = (Dir.Normalize());
}

void FPLightComponent::SetRange(float Ran)
{
	LightConstantBuffer.Range = Ran;
}

void FPLightComponent::SetDiffuse(FPVector4& Diffuse)
{
	LightConstantBuffer.Diffuse = Diffuse;
}

void FPLightComponent::SetAmbient(FPVector4& Ambient)
{
	LightConstantBuffer.Ambient = Ambient;
}

void FPLightComponent::SetLightColor(FPVector4& LightColor)
{
	LightConstantBuffer.LightColor = LightColor;
}

void FPLightComponent::SetSpecular(FPVector4& Specular)
{
	LightConstantBuffer.Specular = Specular;
}

void FPLightComponent::SetAttenuation(FPVector4& Attenuation)
{
	LightConstantBuffer.Attenuation = Attenuation;
}


FPLightComponent::~FPLightComponent()
{
}
