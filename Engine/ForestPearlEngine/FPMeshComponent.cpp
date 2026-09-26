#include "FPMeshComponent.h"
#include "FPAssetManager.h"
#include "FPGameInstance.h"
#include "Define/FPDataDefine.h"
#include <iostream>

FPMeshComponent::FPMeshComponent(FPActor* Owner, std::string MeshPath) : FPPrimitiveComponent(Owner), MeshData(MeshPath)
{
	std::vector<FPVertexBufferData> MeshData = LoadVertexBuffer(MeshPath);
	
	for (FPVertexBufferData Mesh : MeshData)
	{
		VB.push_back(Mesh.VertexBuffer);
		VertexSize.push_back(Mesh.Size);
		Stride = Mesh.Stride;
		Offest = Mesh.Offset;
	}

	if (Material == nullptr) { Material = new FPMaterial(); }
	RegistMeshRenderList();
}

std::vector<FPVertexBufferData> FPMeshComponent::LoadVertexBuffer(std::string MeshPath)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	return AssetManager->GetVertexBuffer(MeshPath);
}

void FPMeshComponent::RegistMeshRenderList()
{
	FPMeshRenderList* MeshRenderList = static_cast<FPMeshRenderList*>(FPGameInstance::Get().GetMeshRenderList());
	RenderItem = MeshRenderList->RegistRenderList();

	RenderItem->Priority = &(this->Priority);
	RenderItem->Active = &(this->isActive);
	RenderItem->VB = &(this->VB);
	RenderItem->isFill = &(this->isFill);
	RenderItem->isCull = &(this->isCull);
	RenderItem->VertexSize = &(this->VertexSize);
	RenderItem->Stride = &(this->Stride);
	RenderItem->Offset = &(this->Offest);
	RenderItem->Location = &(this->WorldTransform.LocationMatrix);
	RenderItem->Rotation = &(this->WorldTransform.RotationMatrix);
	RenderItem->Scale = &(this->WorldTransform.ScaleMatrix);
	RenderItem->Topo = &(this->Topo);
	RenderItem->VertexShader = (this->Material->GetVertexShaderPointer());
	RenderItem->PixelShader = (this->Material->GetPixelShaderPointer());
	RenderItem->VBLayout = (this->Material->GetVBLayoutPointer());
	RenderItem->VertexConst = (this->Material->GetVertexConstPointer());
	RenderItem->PixelConst = (this->Material->GetPixelConstPointer());
}

void FPMeshComponent::SetMaterial(FPMaterial* Material)
{
	this->Material = Material;

	RenderItem->VertexShader = (this->Material->GetVertexShaderPointer());
	RenderItem->PixelShader = (this->Material->GetPixelShaderPointer());
	RenderItem->VBLayout = (this->Material->GetVBLayoutPointer());
	RenderItem->VertexConst = (this->Material->GetVertexConstPointer());
	RenderItem->PixelConst = (this->Material->GetPixelConstPointer());
}

FPMeshComponent::~FPMeshComponent()
{
	FPMeshRenderList* MeshRenderList = static_cast<FPMeshRenderList*>(FPGameInstance::Get().GetMeshRenderList());
	MeshRenderList->UnregistRenderList(RenderItem);
}