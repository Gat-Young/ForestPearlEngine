#include "FPStaticMeshComponent.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include "FPStaticMesh.h"
#include "FPMaterial.h"
#include "FPGizmoNormalLineComponent.h"
#include <iostream>

FPStaticMeshComponent::FPStaticMeshComponent(FPActor* Owner, std::string StaticMeshName) : FPMeshComponent(Owner, StaticMeshName)
{
	//AssetManager로 부터 MeshPath의 StaticMesh_Json 파일을 읽고 Static_Mesh 객체를 반환 받아 저장.
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	StaticMesh = AssetManager->GetStaticMeshData(StaticMeshName);

	GizmoNormalLine = new FPGizmoNormalLineComponent(this->GetOwner(), *(StaticMesh->GetMeshPath()));

	GizmoNormalLine->SetupAttachment(this);

	FPMaterialInterface* StaticMeshMaterial = *(StaticMesh->GetStaticMeshMaterial());
	if (StaticMeshMaterial == nullptr) { Material = new FPMaterial(); }
	RegistMeshRenderList();
}

void FPStaticMeshComponent::SetRenderItemData()
{
	if (RenderItem == nullptr) { std::cout << "[StaticMeshComponent] : 등록 받은 렌더 아이템이 없습니다." << "\n"; }

	RenderItem->Priority = &(this->Priority);
	RenderItem->Active = &(this->isActive);

	RenderItem->MeshPath = StaticMesh->GetMeshPath();

	RenderItem->Location = &(this->WorldTransform.LocationMatrix);
	RenderItem->Rotation = &(this->WorldTransform.RotationMatrix);
	RenderItem->Scale = &(this->WorldTransform.ScaleMatrix);


	FPMaterialInterface* SelectedMaterial = nullptr;

	if (Material != nullptr)
	{
		SelectedMaterial = Material;
	}
	else
	{
		SelectedMaterial = *(StaticMesh->GetStaticMeshMaterial());
	}
	
	RenderItem->VertexShaderPath = SelectedMaterial->GetVertexShader();
	RenderItem->PixelShaderPath = SelectedMaterial->GetPixelShader();
	RenderItem->VertexConstBuffer = SelectedMaterial->GetVertexConstantBufferRenderData();
	RenderItem->PixelConstBuffer = SelectedMaterial->GetPixelConstantBufferRenderData();

}

FTransform FPStaticMeshComponent::GetSocketTransform(const std::string& SocketName) const
{
	FTransform SocketTransform;
	if (StaticMesh->GetSocketTransform(SocketName, SocketTransform))
	{
		//Scale 계산
		SocketTransform.Scale = WorldTransform.Scale * SocketTransform.Scale;

		SocketTransform.QuaternionRotation = FromEuler(SocketTransform.Rotation);
		//Rotation 계산 (부모 사원수 회전 * 로컬 사원수 회전)
		SocketTransform.QuaternionRotation = (WorldTransform.QuaternionRotation * SocketTransform.QuaternionRotation).Normalize();
		SocketTransform.Rotation = SocketTransform.QuaternionRotation.ToEuler();

		//Location 계산 (부모 위치 + Rotate(부모 회전 사원수, (자식 위치 * 부모 크기)) 
		SocketTransform.Location = WorldTransform.Location +
			Rotate(WorldTransform.QuaternionRotation, (SocketTransform.Location * WorldTransform.Scale));

		return SocketTransform;
	}
	return WorldTransform;
}

FPStaticMeshComponent::~FPStaticMeshComponent()
{
}

void FPStaticMeshComponent::SetActive(bool Active)
{
	this->isActive = Active;
	GizmoNormalLine->SetActive(Active);
}

void FPStaticMeshComponent::SetPriority(int Prio)
{
	this->Priority = Prio;
	GizmoNormalLine->SetPriority(Prio);
}

void FPStaticMeshComponent::SetStaticMesh(std::string StaticMeshName)
{
	//AssetManager로 부터 MeshPath의 StaticMesh_Json 파일을 읽고 Static_Mesh 객체를 반환 받아 저장.
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	StaticMesh = AssetManager->GetStaticMeshData(StaticMeshName);
	GizmoNormalLine->SetGizmoMeshPath(*(StaticMesh->GetMeshPath()));
	RegistMeshRenderList();
}
