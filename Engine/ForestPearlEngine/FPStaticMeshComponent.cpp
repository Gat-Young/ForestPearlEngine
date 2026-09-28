#include "FPStaticMeshComponent.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include "FPStaticMesh.h"
#include "FPMaterial.h"
#include <iostream>

FPStaticMeshComponent::FPStaticMeshComponent(FPActor* Owner, std::string MeshPath) : FPMeshComponent(Owner, MeshPath)
{
	//AssetManager로 부터 MeshPath의 StaticMesh_Json 파일을 읽고 Static_Mesh 객체를 반환 받아 저장.
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	//StaticMesh = AssetManager->GetStaticMesh(MeshPath);

	FPMaterialInterface* StaticMeshMaterial = *(StaticMesh->GetStaticMeshMaterial());
	if (StaticMeshMaterial == nullptr) { Material = new FPMaterial(); }
	RegistMeshRenderList();
}

void FPStaticMeshComponent::SetRenderItemData()
{
	if (RenderItem == nullptr) { std::cout << "[StaticMeshComponent] : 등록 받은 렌더 아이템이 없습니다." << "\n"; }

	RenderItem->Priority = &(this->Priority);
	RenderItem->Active = &(this->isActive);
	RenderItem->VB = StaticMesh->GetVBData();
	RenderItem->isFill = &(this->isFill);
	RenderItem->isCull = &(this->isCull);
	RenderItem->VertexSize = StaticMesh->GetVertexSize();
	RenderItem->Stride = StaticMesh->GetStride();
	RenderItem->Offset = StaticMesh->GetOffset();
	RenderItem->Location = &(this->WorldTransform.LocationMatrix);
	RenderItem->Rotation = &(this->WorldTransform.RotationMatrix);
	RenderItem->Scale = &(this->WorldTransform.ScaleMatrix);
	RenderItem->Topo = StaticMesh->GetTopo();

	FPMaterialInterface* SelectedMaterial = nullptr;

	if (Material != nullptr)
	{
		SelectedMaterial = Material;
	}
	else
	{
		SelectedMaterial = *(StaticMesh->GetStaticMeshMaterial());
	}

	RenderItem->VertexShader = (SelectedMaterial->GetVertexShaderPointer());
	RenderItem->PixelShader = (SelectedMaterial->GetPixelShaderPointer());
	RenderItem->VBLayout = (SelectedMaterial->GetVBLayoutPointer());
	RenderItem->VertexConst = (SelectedMaterial->GetVertexConstPointer());
	RenderItem->PixelConst = (SelectedMaterial->GetPixelConstPointer());
}

FPStaticMeshComponent::~FPStaticMeshComponent()
{
}
