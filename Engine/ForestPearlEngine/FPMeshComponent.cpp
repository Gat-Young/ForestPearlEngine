#include "FPMeshComponent.h"
#include "FPAssetManager.h"
#include "FPGameInstance.h"

FPMeshComponent::FPMeshComponent(FPActor* Owner, std::string MeshPath) : FPPrimitiveComponent(Owner), MeshData(MeshPath)
{
	std::vector<std::pair<int, int> > MeshData = LoadVertexBuffer(MeshPath);
	
	for (std::pair<int, int> Mesh : MeshData)
	{
		VBIndex.push_back(Mesh.first);
		VertexSize.push_back(Mesh.second);
	}

	if (Material == nullptr) { Material = new FPMaterial(); }
	RegistMeshRenderList();
}

std::vector<std::pair<int, int> > FPMeshComponent::LoadVertexBuffer(std::string MeshPath)
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
	RenderItem->VBIndex = &(this->VBIndex);
	RenderItem->isFill = &(this->isFill);
	RenderItem->isCull = &(this->isCull);
	RenderItem->VertexSize = &(this->VertexSize);
	RenderItem->Location = &(this->WorldTransform.Location);
	RenderItem->Rotation = &(this->WorldTransform.QuaternionRotation);
	RenderItem->Scale = &(this->WorldTransform.Scale);
	RenderItem->Topo = &(this->Topo);
	RenderItem->VertexShader = (this->Material->GetVertexShaderPointer());
	RenderItem->PixelShader = (this->Material->GetPixelShaderPointer());
	RenderItem->VBLayout = (this->Material->GetVBLayoutPointer());
}

FPMeshComponent::~FPMeshComponent()
{
	FPMeshRenderList* MeshRenderList = static_cast<FPMeshRenderList*>(FPGameInstance::Get().GetMeshRenderList());
	MeshRenderList->UnregistRenderList(RenderItem);
}