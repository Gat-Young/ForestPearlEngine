#include "MeshComponent.h"
#include "AssetManager.h"

MeshComponent::MeshComponent(std::string MeshPath, Transform* transform) : MeshData(MeshPath)
{
	std::pair<int, int> MeshData = LoadVertexBuffer();
	VBIndex = MeshData.first;
	FaceSize = MeshData.second;
	RegistMeshRenderList(transform);
}

std::pair<int, int> MeshComponent::LoadVertexBuffer()
{
	return AssetManager::Get().LordVertexVuffer(MeshData);
}

void MeshComponent::RegistMeshRenderList(Transform* transform)
{
	RenderItem = MeshRenderList::Get().RegistRenderList();

	RenderItem->VBIndex = &(this->VBIndex);
	RenderItem->isFill = &(this->isFill);
	RenderItem->isCull = &(this->isCull);
	RenderItem->FaceSize = &(this->FaceSize);
	RenderItem->transform = transform;
}

MeshComponent::~MeshComponent()
{
	MeshRenderList::Get().UnregistRenderList(RenderItem);
}