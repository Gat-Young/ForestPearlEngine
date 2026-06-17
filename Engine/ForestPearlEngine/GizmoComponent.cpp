#include "GizmoComponent.h"
#include "AssetManager.h"

int GizmoComponent::MakeVertexVuffer(std::vector<GIZMO_COLVTX> GizmoMesh)
{
	std::vector<FPMesh> Vertex;

	for (int i = 0; i < GizmoMesh.size(); ++i)
	{
		Vertex.push_back(FPMesh{ GizmoMesh[i].vPos.x, GizmoMesh[i].vPos.y, GizmoMesh[i].vPos.z, GizmoMesh[i].Color });
	}

	return AssetManager::Get().MakeVertexVuffer(Vertex);
}

void GizmoComponent::MakeGrid(GRIDINFO* grid)
{
	//이미 GizmoData가 있는 컴포넌트라면 생성하지 않음
	if (!GizmoDatas.empty()) return;

	//정적 버퍼 생성.
	int xcnt = (grid->width / (int)grid->scale) + 1;				//가로/세로별 라인 개수.
	int ycnt = (grid->width / (int)grid->scale) + 1;
	int vtxcnt = (xcnt + ycnt) * 2;									//정점개수.

	GizmoDatas.resize(sizeof(GIZMO_COLVTX) * vtxcnt);

	//그리드 시작 위치. (기본. 원점(0,0,0))
	float hx = (grid->width * 0.5f);
	float hy = (grid->height * 0.5f);

	//x 축 라인 생성.
	int k = 0;
	for (int i = 0; i < xcnt; i++, k += 2)
	{
		float sx = -hx;
		float sz = hy - i * grid->scale;		//위에서 아래로 내려옵니다.(+Z --> -Z)

		GizmoDatas[k].vPos = FPVector3{ sx, 0.0f, sz };
		GizmoDatas[k + 1].vPos = FPVector3{ sx + grid->width, 0.0f, sz };

		GizmoDatas[k].Color = (unsigned long)(xcnt / 2 == i) ? COLOR_ARGB(1, 0, 0, 0) : grid->color;
		GizmoDatas[k + 1].Color = (unsigned long)(xcnt / 2 == i) ? COLOR_ARGB(1, 0, 0, 0) : grid->color;
	}

	//z 축 라인 생성
	for (int j = 0; j < ycnt; j++, k += 2)
	{
		float sx = -hx + j * grid->scale;		//왼쪽에서 오른쪽으로..(-X --> +X)
		float sz = hy;

		GizmoDatas[k].vPos = FPVector3{ sx, 0.0f, sz };
		GizmoDatas[k + 1].vPos = FPVector3{ sx, 0.0f, sz - grid->height };

		GizmoDatas[k].Color = (unsigned)(ycnt / 2 == j) ? COLOR_ARGB(1, 0, 0, 0) : grid->color;
		GizmoDatas[k + 1].Color = (unsigned)(ycnt / 2 == j) ? COLOR_ARGB(1, 0, 0, 0) : grid->color;
	}

	//전체 라인개수.
	LineCount = vtxcnt / 2;

	VBIndex = MakeVertexVuffer(GizmoDatas);

	Active = true;
}

void GizmoComponent::MakeAxis(GIZMO_AXISINFO* axis)
{
	//이미 GizmoData가 있는 컴포넌트라면 생성하지 않음
	if (!GizmoDatas.empty()) return;

	//방향 축 문자를 출력 <- DX9이라 GDI로 그려야함 하는 어려움이 있으므로 추후에 구현

	VBIndex = MakeVertexVuffer(GizmoDatas);
}

void GizmoComponent::RegistGizmoRenderList(FPVector3* Position, FPVector3* Rotation, FPVector3* Scale)
{
	RenderItem = GizmoRenderList::Get().RegistRenderList();

	RenderItem->Active = &(this->Active);
	RenderItem->VBIndex = &(this->VBIndex);
	RenderItem->LineCount = &(this->LineCount);
	RenderItem->Position = Position;
	RenderItem->Rotation = Rotation;
	RenderItem->Scale = Scale;
}

GizmoComponent::~GizmoComponent()
{
	GizmoRenderList::Get().UnregistRenderList(RenderItem);
}
