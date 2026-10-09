#include "FPMeshComponent.h"
#include "FPAssetManager.h"
#include "FPGameInstance.h"
#include "Define/FPDataDefine.h"
#include <iostream>

FPMeshComponent::FPMeshComponent(FPActor* Owner, std::string MeshName) : FPPrimitiveComponent(Owner), MeshName(MeshName)
{

}

void FPMeshComponent::RegistMeshRenderList()
{
	FPMeshRenderList* MeshRenderList = static_cast<FPMeshRenderList*>(FPGameInstance::Get().GetMeshRenderList());
	RenderItem = MeshRenderList->RegistRenderList();

	SetRenderItemData();
}

void FPMeshComponent::SetMaterial(FPMaterialInterface* Material)
{
	this->Material = Material;
	SetRenderItemData();
}

FPMeshComponent::~FPMeshComponent()
{
	FPMeshRenderList* MeshRenderList = static_cast<FPMeshRenderList*>(FPGameInstance::Get().GetMeshRenderList());
	MeshRenderList->UnregistRenderList(RenderItem);
}