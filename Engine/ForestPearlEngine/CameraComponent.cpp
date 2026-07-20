#include "CameraComponent.h"
#include "CameraList.h"

CameraComponent::CameraComponent(FPActor* Owner) : FPPrimitiveComponent(Owner)
{
	RegistCamera();
}

CameraComponent::~CameraComponent()
{
	CameraList::Get().UnregistRenderList(CamItem);
}

void CameraComponent::RegistCamera()
{
	CamItem = CameraList::Get().RegistRenderList();

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
