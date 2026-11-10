#include "FPCameraComponent.h"
#include "FPGameInstance.h"
#include "FPGameProjectSetting.h"


FPCameraComponent::FPCameraComponent(FPActor* Owner) : FPSceneComponent(Owner)
{
	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	this->Aspect = GameProjectSetting->GetDisplayAspect();
	RegistCamera();
}

FPCameraComponent::~FPCameraComponent()
{
	FPCameraList* CameraList = static_cast<FPCameraList*>(FPGameInstance::Get().GetCameraList());
	CameraList->UnregistRenderList(CamItem);
}

void FPCameraComponent::RegistCamera()
{
	FPCameraList* CameraList = static_cast<FPCameraList*>(FPGameInstance::Get().GetCameraList());
	CamItem = CameraList->RegistRenderList();

	CamItem->Location = &(this->WorldTransform.LocationMatrix);
	CamItem->Rotation = &(this->WorldTransform.RotationMatrix);
	CamItem->Scale = &(this->WorldTransform.ScaleMatrix);

	CamItem->View = &(this->ViewMatrix);
	CamItem->Projection = &(this->ProjectionMatrix);

	CamItem->Active = &(this->Active);
	CamItem->TripleCam = &(this->TripleCame);

	CamItem->RenderTarget = &(this->RenderTarget);
	CamItem->DepthStencil = &(this->DepthStencil);
}

void FPCameraComponent::Tick()
{
	__super::Tick();

	//ViewMatrix = MatrixLookAtLH(WorldTransform.Location, LookAt, Up);

	ViewMatrix = MatrtixTranslation(-WorldTransform.Location) * WorldTransform.RotationMatrix.Transpose();
	ProjectionMatrix = MatrixPerspectiveFovLH(Fov, Aspect, Zn, Zf);
	
}

void FPCameraComponent::OnTripleCam()
{
	TripleCame = !TripleCame;
}

void FPCameraComponent::SetRenderTarget(std::string& RenderTarget)
{
	this->RenderTarget = RenderTarget;
}

std::string FPCameraComponent::GetRenderTarget()
{
	return this->RenderTarget;
}

void FPCameraComponent::SetDepthStencil(std::string& DepthStencil)
{
	this->DepthStencil = DepthStencil;
}

std::string FPCameraComponent::GetDepthStencil()
{
	return this->DepthStencil;
}
