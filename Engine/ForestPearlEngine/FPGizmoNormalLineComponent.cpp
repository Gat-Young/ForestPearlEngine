#include "FPGizmoNormalLineComponent.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include "FPMeshNormalRenderList.h"
#include <iostream>

void FPGizmoNormalLineComponent::RegistNormalRenderList()
{
	FPMeshNormalRenderList* NormalRenderList = static_cast<FPMeshNormalRenderList*>(FPGameInstance::Get().GetNormalRenderList());
	RenderItem = NormalRenderList->RegistRenderList();

	if (RenderItem == nullptr) { std::cout << "[GizmoNormalLineComponent] : 등록 받은 렌더 아이템이 없습니다." << "\n"; return; }

	RenderItem->Active = &(this->isActive);
	RenderItem->MeshPath = &(MeshPath);
	RenderItem->NormalLength = &(NormalLength);

	RenderItem->Location = &(this->WorldTransform.LocationMatrix);
	RenderItem->Rotation = &(this->WorldTransform.RotationMatrix);
	RenderItem->Scale = &(this->WorldTransform.ScaleMatrix);

	FPMaterialInterface* SelectedMaterial = Material;

	RenderItem->VertexShaderPath = SelectedMaterial->GetVertexShader();
	RenderItem->PixelShaderPath = SelectedMaterial->GetPixelShader();
}

FPGizmoNormalLineComponent::FPGizmoNormalLineComponent(FPActor* Owner, std::string& StaticMeshPath) : FPPrimitiveComponent(Owner), MeshPath(StaticMeshPath)
{
	if (Material == nullptr) { Material = new FPMaterial(); }
	Material->SetVertexShader("NormalDebugVertexShader");
	Material->SetPixelShader("NormalDebugPixelShader");
	RegistNormalRenderList();
}

FPGizmoNormalLineComponent::~FPGizmoNormalLineComponent()
{
}

void FPGizmoNormalLineComponent::SetMeshPath(const std::string& StaticMeshPath)
{
	this->MeshPath = StaticMeshPath;
}
