#include "MeshComponent.h"
#include "AssetManager.h"

MeshComponent::MeshComponent(FPActor* Owner, std::string MeshPath) : FPPrimitiveComponent(Owner), MeshData(MeshPath)
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

std::vector<std::pair<int, int> > MeshComponent::LoadVertexBuffer(std::string MeshPath)
{
	return AssetManager::Get().LoadVertexBuffer(MeshPath);
}

void MeshComponent::RegistMeshRenderList()
{
	RenderItem = MeshRenderList::Get().RegistRenderList();

	RenderItem->VBIndex = &(this->VBIndex);
	RenderItem->isFill = &(this->isFill);
	RenderItem->isCull = &(this->isCull);
	RenderItem->VertexSize = &(this->VertexSize);
	RenderItem->Location = &(this->WorldTransform.Location);
	RenderItem->Rotation = &(this->WorldTransform.QuaternionRotation);
	RenderItem->Scale = &(this->WorldTransform.Scale);
	RenderItem->VertexShader = (this->Material->GetVertexShaderPointer());
	RenderItem->PixelShader = (this->Material->GetPixelShaderPointer());
	RenderItem->VBLayout = (this->Material->GetVBLayoutPointer());
}

MeshComponent::~MeshComponent()
{
	MeshRenderList::Get().UnregistRenderList(RenderItem);
}