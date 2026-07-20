#include "GizmoComponent.h"
#include "AssetManager.h"

int GizmoComponent::MakeVertexBuffer(std::vector<GIZMO_VERTEX> GizmoMesh)
{
	std::vector<FPMesh> Vertex;

	for (int i = 0; i < GizmoMesh.size(); ++i)
	{
		Vertex.push_back(FPMesh{ GizmoMesh[i].x, GizmoMesh[i].y, GizmoMesh[i].z, GizmoMesh[i].r, GizmoMesh[i].g, GizmoMesh[i].b, GizmoMesh[i].a});
	}

	return AssetManager::Get().MakeVertexBuffer(Vertex);
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
	VertexSize = vtxcnt;

	VBIndex = MakeVertexBuffer(GizmoDatas);

	Active = true;
}

void GizmoComponent::MakeAxis(GIZMO_AXISINFO* axis)
{
	//이미 GizmoData가 있는 컴포넌트라면 생성하지 않음
	if (!GizmoDatas.empty()) return;

	//방향 축 문자를 출력 <- DX9이라 GDI로 그려야함 하는 어려움이 있으므로 추후에 구현

	VBIndex = MakeVertexBuffer(GizmoDatas);
}

void GizmoComponent::RegistGizmoRenderList()
{
	RenderItem = GizmoRenderList::Get().RegistRenderList();

	RenderItem->Active = &(this->Active);
	RenderItem->VBIndex = &(this->VBIndex);
	RenderItem->VertexSize = &(this->VertexSize);
	RenderItem->Location = &(this->WorldTransform.Location);
	RenderItem->Rotation = &(this->WorldTransform.QuaternionRotation);
	RenderItem->Scale = &(this->WorldTransform.Scale);
	RenderItem->VertexShader = (this->Material->GetVertexShaderPointer());
	RenderItem->PixelShader = (this->Material->GetPixelShaderPointer());
	RenderItem->VBLayout = (this->Material->GetVBLayoutPointer());
}

GizmoComponent::GizmoComponent(FPActor* Owner) : FPPrimitiveComponent(Owner)
{
	GRIDINFO grid;
	grid.width = 100;
	grid.height = 100;

	MakeGrid(&grid);

	if (Material == nullptr) { Material = new FPMaterial(); }

	RegistGizmoRenderList();
}

GizmoComponent::~GizmoComponent()
{
	GizmoRenderList::Get().UnregistRenderList(RenderItem);
}
