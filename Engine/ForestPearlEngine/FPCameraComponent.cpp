#include "FPCameraComponent.h"
#include "FPGameInstance.h"

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

	CamItem->Location = &(this->WorldTransform.Location);
	CamItem->Rotation = &(this->WorldTransform.QuaternionRotation);
	CamItem->Scale = &(this->WorldTransform.Scale);

	CamItem->LookAt = &(this->LookAt);
	CamItem->Up = &(this->Up);

	CamItem->Fov = &(this->Fov);
	CamItem->Aspect = &(this->Aspect);
	CamItem->Zn = &(this->Zn);
	CamItem->Zf = &(this->Zf);

	CamItem->Active = &(this->Active);
}
