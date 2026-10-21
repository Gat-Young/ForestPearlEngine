#include "FPGizmoComponent.h"
#include "FPAssetManager.h"
#include "FPGameInstance.h"
#include "Renderers/FPRenderingCommon.h"

void FPGizmoComponent::MakeVertexBuffer(std::vector<FPGIZMO_VERTEX>& GizmoMesh, const std::string& GizmoMeshPath)
{
	std::vector<VERTEX> Vertex;

	for (int i = 0; i < GizmoMesh.size(); ++i)
	{
		VERTEX GizmoVertexData;
		GizmoVertexData.PositionVertex.x = GizmoMesh[i].x;
		GizmoVertexData.PositionVertex.y = GizmoMesh[i].y;
		GizmoVertexData.PositionVertex.z = GizmoMesh[i].z;
		GizmoVertexData.ColorVertex.r = GizmoMesh[i].r;
		GizmoVertexData.ColorVertex.g = GizmoMesh[i].g;
		GizmoVertexData.ColorVertex.b = GizmoMesh[i].b;
		GizmoVertexData.ColorVertex.a = GizmoMesh[i].a;

		Vertex.push_back(GizmoVertexData);
	}

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	AssetManager->MakeGizmoVertexBuffer(Vertex, GizmoMeshPath);
}

void FPGizmoComponent::RegistGizmoRenderList()
{
	FPGizmoRenderList* GizmoRenderList = static_cast<FPGizmoRenderList*>(FPGameInstance::Get().GetGizmoRenderList());
	RenderItem = GizmoRenderList->RegistRenderList();

	if (RenderItem == nullptr) { std::cout << "[GizmoComponent] : 등록 받은 렌더 아이템이 없습니다." << "\n"; return; }

	RenderItem->Priority = &(this->Priority);
	RenderItem->Active = &(this->isActive);
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

FPGizmoComponent::FPGizmoComponent(FPActor* Owner, const std::string& GizmoMeshPath) : GizmoMeshPath(GizmoMeshPath), FPPrimitiveComponent(Owner)
{
	if (Material == nullptr) { Material = new FPMaterial(); }
	Material->SetVertexShader("DefaultGizmoVertexShader");
	Material->SetPixelShader("DefaultGizmoPixelShader");
}


FPGizmoComponent::~FPGizmoComponent()
{
	FPGizmoRenderList* GizemoRenderList = static_cast<FPGizmoRenderList*>(FPGameInstance::Get().GetGizmoRenderList());
	GizemoRenderList->UnregistRenderList(RenderItem);
}
