#include "FPCameraComponent.h"
#include "FPGameInstance.h"
#include "FPGameProjectSetting.h"

FPCameraComponent::FPCameraComponent(FPActor* Owner) : FPPrimitiveComponent(Owner)
{
	ViewPort.TopLeftX = 0.0f;
	ViewPort.TopLeftY = 0.0f;

	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	ViewPort.Width = GameProjectSetting->GetWinWidth();
	ViewPort.Height = GameProjectSetting->GetWinHeight();

	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

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

	CamItem->ViewPort = &(this->ViewPort);

	CamItem->Active = &(this->Active);
}

void FPCameraComponent::Tick()
{
	__super::Tick();

	ViewMatrix = MatrixLookAtLH(WorldTransform.Location, LookAt, Up);
	ProjectionMatrix = MatrixPerspectiveFovLH(Fov, Aspect, Zn, Zf);
	
}

void FPCameraComponent::SetViewPortSetting(float TopLeftX, float TopLeftY, float Width, float Height, float MinDepth, float MaxDepth)
{

	this->ViewPort.TopLeftX = TopLeftX;
	this->ViewPort.TopLeftY = TopLeftY;
	
	this->ViewPort.Width = Width;
	this->ViewPort.Height = Height;

	this->ViewPort.MinDepth = MinDepth;
	this->ViewPort.MaxDepth = MaxDepth;
}
