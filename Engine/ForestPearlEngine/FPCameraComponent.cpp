#include "FPCameraComponent.h"
#include "FPGameInstance.h"
#include "FPGameProjectSetting.h"

FPCameraComponent::FPCameraComponent(FPActor* Owner) : FPPrimitiveComponent(Owner)
{
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
}

void FPCameraComponent::Tick()
{
	__super::Tick();

	ViewMatrix = MatrixLookAtLH(WorldTransform.Location, LookAt, Up);
	ProjectionMatrix = MatrixPerspectiveFovLH(Fov, Aspect, Zn, Zf);
	
}