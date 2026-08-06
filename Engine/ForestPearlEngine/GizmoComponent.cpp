#include "GizmoComponent.h"
#include "FPAssetLoader.h"
#include "FPGameInstance.h"
#include "Define/FPDataDefine.h"

int GizmoComponent::MakeVertexBuffer(std::vector<GIZMO_VERTEX> GizmoMesh)
{
	std::vector<VERTEX> Vertex;

	for (int i = 0; i < GizmoMesh.size(); ++i)
	{
		Vertex.push_back(VERTEX{ GizmoMesh[i].x, GizmoMesh[i].y, GizmoMesh[i].z, GizmoMesh[i].r, GizmoMesh[i].g, GizmoMesh[i].b, GizmoMesh[i].a});
	}

	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	return AssetLoader->MakeVertexBuffer(Vertex);
}

void GizmoComponent::MakeGrid(GRIDINFO* grid)
{
	//이미 GizmoData가 있는 컴포넌트라면 생성하지 않음
	if (!GizmoDatas.empty()) return;

	//정적 버퍼 생성.
	int xcnt = (grid->width / (int)grid->scale) + 1;				//가로/세로별 라인 개수.
	int ycnt = (grid->width / (int)grid->scale) + 1;
	int vtxcnt = (xcnt + ycnt) * 2;									//정점개수.

	GizmoDatas.resize(sizeof(GIZMO_VERTEX) * vtxcnt);

	//그리드 시작 위치. (기본. 원점(0,0,0))
	float hx = (grid->width * 0.5f);
	float hy = (grid->height * 0.5f);

	//x 축 라인 생성.
	int k = 0;
	for (int i = 0; i < xcnt; i++, k += 2)
	{
		float sx = -hx;
		float sz = hy - i * grid->scale;		//위에서 아래로 내려옵니다.(+Z --> -Z)

		GizmoDatas[k].x = sx;
		GizmoDatas[k].y = 0.0f;
		GizmoDatas[k].z = sz;

		GizmoDatas[k + 1].x = sx + grid->width;
		GizmoDatas[k + 1].y = 0.0f;
		GizmoDatas[k + 1].z = sz;

		GizmoDatas[k].r = (unsigned long)(xcnt / 2 == i) ? 0 : grid->r;
		GizmoDatas[k].g = (unsigned long)(xcnt / 2 == i) ? 0 : grid->g;
		GizmoDatas[k].b = (unsigned long)(xcnt / 2 == i) ? 0 : grid->b;
		GizmoDatas[k].a = (unsigned long)(xcnt / 2 == i) ? 1 : grid->a;

		GizmoDatas[k+1].r = (unsigned long)(xcnt / 2 == i) ? 0 : grid->r;
		GizmoDatas[k+1].g = (unsigned long)(xcnt / 2 == i) ? 0 : grid->g;
		GizmoDatas[k+1].b = (unsigned long)(xcnt / 2 == i) ? 0 : grid->b;
		GizmoDatas[k+1].a = (unsigned long)(xcnt / 2 == i) ? 1 : grid->a;
	}

	//z 축 라인 생성
	for (int j = 0; j < ycnt; j++, k += 2)
	{
		float sx = -hx + j * grid->scale;		//왼쪽에서 오른쪽으로..(-X --> +X)
		float sz = hy;

		GizmoDatas[k].x = sx;
		GizmoDatas[k].y = 0.0f;
		GizmoDatas[k].z = sz;

		GizmoDatas[k + 1].x = sx;
		GizmoDatas[k + 1].y = 0.0f;
		GizmoDatas[k + 1].z = sz - grid->height;

		GizmoDatas[k].r = (unsigned long)(ycnt / 2 == j) ? 0 : grid->r;
		GizmoDatas[k].g = (unsigned long)(ycnt / 2 == j) ? 0 : grid->g;
		GizmoDatas[k].b = (unsigned long)(ycnt / 2 == j) ? 0 : grid->b;
		GizmoDatas[k].a = (unsigned long)(ycnt / 2 == j) ? 1 : grid->a;

		GizmoDatas[k + 1].r = (unsigned long)(ycnt / 2 == j) ? 0 : grid->r;
		GizmoDatas[k + 1].g = (unsigned long)(ycnt / 2 == j) ? 0 : grid->g;
		GizmoDatas[k + 1].b = (unsigned long)(ycnt / 2 == j) ? 0 : grid->b;
		GizmoDatas[k + 1].a = (unsigned long)(ycnt / 2 == j) ? 1 : grid->a;
	}

	//정점 개수.
	VertexSize.push_back(vtxcnt);

	VBIndex.push_back(MakeVertexBuffer(GizmoDatas));

	isActive = true;
}

void GizmoComponent::MakeAxis(GIZMO_AXISINFO* axis)
{
	//이미 GizmoData가 있는 컴포넌트라면 생성하지 않음
	if (!GizmoDatas.empty()) return;

	//정적 버퍼 생성.
	int vtxcnt = 6;								//정점개수.

	GizmoDatas.resize(sizeof(GIZMO_VERTEX) * vtxcnt);

	//x 축 라인 생성.
	GizmoDatas[0].x = 0.0f;
	GizmoDatas[0].y = 0.0f;
	GizmoDatas[0].z = 0.0f;
	GizmoDatas[0].r = 1.0f;
	GizmoDatas[0].g = 0.0f;
	GizmoDatas[0].b = 0.0f;
	GizmoDatas[0].a = 1.0f;


	GizmoDatas[1].x = axis->length * axis->scale;
	GizmoDatas[1].y = 0.0f;
	GizmoDatas[1].z = 0.0f;
	GizmoDatas[1].r = 1.0f;
	GizmoDatas[1].g = 0.0f;
	GizmoDatas[1].b = 0.0f;
	GizmoDatas[1].a = 1.0f;

	//y축 생성
	GizmoDatas[2].x = 0.0f;
	GizmoDatas[2].y = 0.0f;
	GizmoDatas[2].z = 0.0f;
	GizmoDatas[2].r = 0.0f;
	GizmoDatas[2].g = 1.0f;
	GizmoDatas[2].b = 0.0f;
	GizmoDatas[2].a = 1.0f;

	GizmoDatas[3].x = 0.0f;
	GizmoDatas[3].y = axis->length * axis->scale;
	GizmoDatas[3].z = 0.0f;
	GizmoDatas[3].r = 0.0f;
	GizmoDatas[3].g = 1.0f;
	GizmoDatas[3].b = 0.0f;
	GizmoDatas[3].a = 1.0f;

	//z축 생성
	GizmoDatas[4].x = 0.0f;
	GizmoDatas[4].y = 0.0f;
	GizmoDatas[4].z = 0.0f;
	GizmoDatas[4].r = 0.0f;
	GizmoDatas[4].g = 0.0f;
	GizmoDatas[4].b = 1.0f;
	GizmoDatas[4].a = 1.0f;

	GizmoDatas[5].x = 0.0f;
	GizmoDatas[5].y = 0.0f;
	GizmoDatas[5].z = axis->length * axis->scale;
	GizmoDatas[5].r = 0.0f;
	GizmoDatas[5].g = 0.0f;
	GizmoDatas[5].b = 1.0f;
	GizmoDatas[5].a = 1.0f;


	//정점 개수.
	VertexSize.push_back(vtxcnt);

	isActive = true;

	VBIndex.push_back(MakeVertexBuffer(GizmoDatas));
}

void GizmoComponent::RegistGizmoRenderList()
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

GizmoComponent::GizmoComponent(FPActor* Owner) : FPPrimitiveComponent(Owner)
{
	if (Material == nullptr) { Material = new FPMaterial(); }


	RegistGizmoRenderList();
}

GizmoComponent::~GizmoComponent()
{
	FPMeshRenderList* MeshRenderList = static_cast<FPMeshRenderList*>(FPGameInstance::Get().GetMeshRenderList());
	MeshRenderList->UnregistRenderList(RenderItem);
}
