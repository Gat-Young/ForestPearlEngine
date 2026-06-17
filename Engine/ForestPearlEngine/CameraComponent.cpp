#include "CameraComponent.h"
#include "CameraList.h"

CameraComponent::~CameraComponent()
{
	CameraList::Get().UnregistRenderList(CamItem);
}

void CameraComponent::RegistCamera()
{
	CamItem = CameraList::Get().RegistRenderList();

	CamItem->Position = &(this->Position);
	CamItem->Rotation = &(this->Rotation);
	CamItem->Scale = &(this->Scale);

	CamItem->LookAt = &(this->LookAt);
	CamItem->Up = &(this->Up);

	CamItem->Fov = &(this->Fov);
	CamItem->Aspect = &(this->Aspect);
	CamItem->Zn = &(this->Zn);
	CamItem->Zf = &(this->Zf);

	CamItem->Active = &(this->Active);

}
