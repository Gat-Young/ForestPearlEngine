#include "MeshComponent.h"
#include "AssetManager.h"

MeshComponent::MeshComponent(std::string MeshPath) : MeshData(MeshPath)
{
	VBIndex = LoadVertexBuffer();
	RegistMeshRenderList();
}

int MeshComponent::LoadVertexBuffer()
{
	return AssetManager::Get().LordVertexVuffer(MeshData);
}

void MeshComponent::RegistMeshRenderList()
{
	RenderItem = MeshRenderList::Get().RegistRenderList();

	RenderItem->VBIndex = &(this->VBIndex);
	RenderItem->isFill = &(this->isFill);
}

MeshComponent::~MeshComponent()
{
	MeshRenderList::Get().UnregistRenderList(RenderItem);
}