#include "MeshComponent.h"
#include "AssetManager.h"

MeshComponent::MeshComponent(FPActor* Owner, std::string MeshPath) : FPPrimitiveComponent(Owner), MeshData(MeshPath)
{
	std::pair<int, int> MeshData = LoadVertexBuffer();
	VBIndex = MeshData.first;
	FaceSize = MeshData.second;
	RegistMeshRenderList();
}

std::pair<int, int> MeshComponent::LoadVertexBuffer()
{
	return AssetManager::Get().LordVertexVuffer(MeshData);
}

void MeshComponent::RegistMeshRenderList()
{
	RenderItem = MeshRenderList::Get().RegistRenderList();

	RenderItem->VBIndex = &(this->VBIndex);
	RenderItem->isFill = &(this->isFill);
	RenderItem->isCull = &(this->isCull);
	RenderItem->FaceSize = &(this->FaceSize);
	RenderItem->Location = &(this->WorldTransform.Location);
	RenderItem->Rotation = &(this->WorldTransform.Rotation);
	RenderItem->Scale = &(this->WorldTransform.Scale);
}

MeshComponent::~MeshComponent()
{
	MeshRenderList::Get().UnregistRenderList(RenderItem);
}