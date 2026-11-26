#include "FPCameraComponent.h"
#include "FPGameInstance.h"
#include "FPGameProjectSetting.h"
#include "FPAssetManager.h"
#include "FPMaterialInterface.h"
#include <iostream>


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

	//PostProcess Material 해제
	delete PostProcessMaterial;
	PostProcessMaterial = nullptr;
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
	CamItem->PostProcessMaterial = &(this->PostProcessMaterial);

	CamItem->Fov = &(this->Fov);
	CamItem->Aspect = &(this->Aspect);
	CamItem->Zn = &(this->Zn);
	CamItem->Zf = &(this->Zf);
	CamItem->ProjectionMode = &(this->ProjectionMode);
	CamItem->OrthoWidth = &(this->OrthoWidth);
}

void FPCameraComponent::Tick()
{
	__super::Tick();

	//ViewMatrix = MatrixLookAtLH(WorldTransform.Location, LookAt, Up);

	ViewMatrix = MatrtixTranslation(-WorldTransform.Location) * Transpose(WorldTransform.RotationMatrix);
	RecalculateProjectionMatrix();
	
}

void FPCameraComponent::SetPostProcessMaterial(std::string MaterialName)
{
	//기존 PostProcess Material 해제
	delete PostProcessMaterial;
	PostProcessMaterial = nullptr;

	if (MaterialName.empty()) return;

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	//MakeMaterial은 없는 이름이면 DefaultMaterial로 대체하므로 이름을 먼저 확인
	if (!AssetManager->HasMaterialData(MaterialName))
	{
		std::cout << "PostProcess Material not loaded: " << MaterialName << "\n";
		return;
	}

	//Vertex Shader가 DefaultQuad2DVertexShader로 고정된 PostProcess용 Material 생성
	PostProcessMaterial = AssetManager->MakeMaterial(MaterialName, true);
}

FPMaterialInterface* FPCameraComponent::GetPostProcessMaterial() const
{
	return this->PostProcessMaterial;
}

void FPCameraComponent::RecalculateProjectionMatrix()
{
	if (ProjectionMode == Perspective)
	{
		ProjectionMatrix = MatrixPerspectiveFovLH(Fov, Aspect, Zn, Zf);
	}
	else
	{
		float HalfWidth = OrthoWidth * 0.5f;
		float HalfHeight = HalfWidth / Aspect;
		ProjectionMatrix = MatrixOrthographicOffCenterLH(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, Zn, Zf);
	}
}

void FPCameraComponent::SetProjectionMode(FPProjectionMode Mode)
{
	this->ProjectionMode = Mode;
	RecalculateProjectionMatrix();
}