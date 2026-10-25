#include "FPLightComponentBase.h"
#include "FPGameInstance.h"
#include <iostream>

FPLightComponentBase::FPLightComponentBase(FPActor* Owner) : FPSceneComponent(Owner)
{
}

void FPLightComponentBase::RegistLightRenderList()
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
	RenderItem->VertexConstBuffer = &(this->VertexConstantBuffer);
	RenderItem->PixelConstBuffer = &(this->PixelConstantBuffer);
}

//Set한 이후 버퍼 데이터를 업데이트
void FPLightComponentBase::SetDirection(FPVector3& Dir)
{
	//방향은 Normalize 음? 이건 근데 Forward 줘도 되는 부분이 아닌가? 뭐 다르게 가져가면 그건 그거 대로 좋은 걸 지도?
	LightConstantBuffer.Direction = -Dir.Normalize();
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, LightConstantBuffer);
}

void FPLightComponentBase::SetRange(float Ran)
{
	LightConstantBuffer.Range = Ran;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, LightConstantBuffer);
}

void FPLightComponentBase::SetDiffuse(FPVector4& Diffuse)
{
	LightConstantBuffer.Diffuse = Diffuse;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, LightConstantBuffer);
}

void FPLightComponentBase::SetAmbient(FPVector4& Ambient)
{
	LightConstantBuffer.Ambient = Ambient;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, LightConstantBuffer);
}

void FPLightComponentBase::SetLightColor(FPVector4& LightColor)
{
	LightConstantBuffer.LightColor = LightColor;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, LightConstantBuffer);
}

void FPLightComponentBase::SetSpecular(FPVector4& Specular)
{
	LightConstantBuffer.Specular = Specular;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, LightConstantBuffer);
}

void FPLightComponentBase::SetAttenuation(FPVector4& Attenuation)
{
	LightConstantBuffer.Attenuation = Attenuation;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, LightConstantBuffer);
}


FPLightComponentBase::~FPLightComponentBase()
{
}
