#include "FPDirectionalLightComponent.h"
#include "FPGameInstance.h"

#include <iostream>

void FPDirectionalLightComponent::RegistLightRenderList()
{
	//Light Render List에 등록
	FPLightRenderList* LightRenderList = static_cast<FPLightRenderList*>(FPGameInstance::Get().GetLightRenderList());
	RenderItem = LightRenderList->RegistRenderList();

	if (RenderItem == nullptr) { std::cout << "[DirectionalLightComponent] : 등롭 받은 렌더 아이템이 없습니다." << "\n"; return; }

	RenderItem->Active = &(this->isActive);
	RenderItem->Range = &(this->DirectionalLightConstantBuffer.Range);
	RenderItem->Direction = &(this->DirectionalLightConstantBuffer.Direction);
	RenderItem->Location = &(this->WorldTransform.LocationMatrix);
	RenderItem->Rotation = &(this->WorldTransform.RotationMatrix);
	RenderItem->Scale = &(this->WorldTransform.ScaleMatrix);
	RenderItem->VertexConstBuffer = &(this->VertexConstantBuffer);
	RenderItem->PixelConstBuffer = &(this->PixelConstantBuffer);
}

FPDirectionalLightComponent::FPDirectionalLightComponent(FPActor* Ownder) : FPLightComponent(Owner)
{
	//상수 버퍼를 정점 상수 버퍼에 등록
	FPConstantBufferRenderDataUtil::AddConstantBuffer(VertexConstantBuffer, 0, DirectionalLightConstantBuffer);
	RegistLightRenderList();
}

FPDirectionalLightComponent::~FPDirectionalLightComponent()
{
}

//Set한 이후 버퍼 데이터를 업데이트
void FPDirectionalLightComponent::SetDirection(FPVector3& Dir)
{
	//방향은 Normalize 음? 이건 근데 Forward 줘도 되는 부분이 아닌가? 뭐 다르게 가져가면 그건 그거 대로 좋은 걸 지도?
	DirectionalLightConstantBuffer.Direction = Dir.Normalize();
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, DirectionalLightConstantBuffer);
}

void FPDirectionalLightComponent::SetRange(float Ran)
{
	DirectionalLightConstantBuffer.Range = Ran;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, DirectionalLightConstantBuffer);
}

void FPDirectionalLightComponent::SetDiffuse(FPVector4& Diffuse)
{
	DirectionalLightConstantBuffer.Diffuse = Diffuse;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, DirectionalLightConstantBuffer);
}

void FPDirectionalLightComponent::SetAmbient(FPVector4& Ambient)
{
	DirectionalLightConstantBuffer.Ambient = Ambient;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, DirectionalLightConstantBuffer);
}