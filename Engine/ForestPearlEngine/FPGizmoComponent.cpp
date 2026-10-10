#include "FPGizmoComponent.h"
#include "FPAssetLoader.h"
#include "FPGameInstance.h"
#include "Define/FPDataDefine.h"

void FPGizmoComponent::MakeVertexBuffer(std::vector<FPGIZMO_VERTEX>& GizmoMesh, const std::string& GizmoMeshPath)
{
	std::vector<VERTEX> Vertex;

	for (int i = 0; i < GizmoMesh.size(); ++i)
	{
		VERTEX GizmoVertexData;
		GizmoVertexData.x = GizmoMesh[i].x;
		GizmoVertexData.y = GizmoMesh[i].y;
		GizmoVertexData.z = GizmoMesh[i].z;
		GizmoVertexData.r = GizmoMesh[i].r;
		GizmoVertexData.g = GizmoMesh[i].g;
		GizmoVertexData.b = GizmoMesh[i].b;
		GizmoVertexData.a = GizmoMesh[i].a;

		Vertex.push_back(GizmoVertexData);
	}

	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->MakeGizmoVertexBuffer(Vertex, GizmoMeshPath);
}

void FPGizmoComponent::RegistGizmoRenderList()
{
	FPGizmoRenderList* GizmoRenderList = static_cast<FPGizmoRenderList*>(FPGameInstance::Get().GetGizmoRenderList());
	RenderItem = GizmoRenderList->RegistRenderList();

	if (RenderItem == nullptr) { std::cout << "[GizmoComponent] : 등록 받은 렌더 아이템이 없습니다." << "\n"; return; }

	RenderItem->Priority = &(this->Priority);
	RenderItem->Active = &(this->isActive);

	RenderItem->ClassName = &(this->GizmoClass);
	RenderItem->MeshPath = &(GizmoMeshPath);

	RenderItem->Location = &(this->WorldTransform.LocationMatrix);
	RenderItem->Rotation = &(this->WorldTransform.RotationMatrix);
	RenderItem->Scale = &(this->WorldTransform.ScaleMatrix);

	FPMaterialInterface* SelectedMaterial = Material;

	RenderItem->VertexShaderPath = SelectedMaterial->GetVertexShader();
	RenderItem->PixelShaderPath = SelectedMaterial->GetPixelShader();
	RenderItem->VertexConstBuffer = SelectedMaterial->GetVertexConstantBufferRenderData();
	RenderItem->PixelConstBuffer = SelectedMaterial->GetPixelConstantBufferRenderData();

}

FPGizmoComponent::FPGizmoComponent(FPActor* Owner,const std::string& GizmoClass ,const std::string& GizmoMeshPath) : GizmoMeshPath(GizmoMeshPath), FPPrimitiveComponent(Owner), GizmoClass(GizmoClass)
{
	if (Material == nullptr) { Material = new FPMaterial(); }
	Material->SetVertexShader("DefaultGizmoVertexShader.vso");
	Material->SetPixelShader("DefaultGizmoPixelShader.vso");
}

FPGizmoComponent::~FPGizmoComponent()
{
	FPGizmoRenderList* GizemoRenderList = static_cast<FPGizmoRenderList*>(FPGameInstance::Get().GetGizmoRenderList());
	GizemoRenderList->UnregistRenderList(RenderItem);
}
