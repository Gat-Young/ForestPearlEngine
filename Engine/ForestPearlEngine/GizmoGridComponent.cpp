#include "GizmoGridComponent.h"

GizmoGridComponent::GizmoGridComponent(FPActor* Owner, std::string GizmoMeshPath) : GizmoComponent(Owner, GizmoMeshPath)
{
	GIZMO_GRIDINFO axis;
	MakeGrid(&axis);
	RegistGizmoRenderList();
}

GizmoGridComponent::~GizmoGridComponent()
{
}


void GizmoGridComponent::MakeGrid(GIZMO_GRIDINFO* grid)
{
	std::vector<GIZMO_VERTEX> GizmoDatas;

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

		GizmoDatas[k + 1].r = (unsigned long)(xcnt / 2 == i) ? 0 : grid->r;
		GizmoDatas[k + 1].g = (unsigned long)(xcnt / 2 == i) ? 0 : grid->g;
		GizmoDatas[k + 1].b = (unsigned long)(xcnt / 2 == i) ? 0 : grid->b;
		GizmoDatas[k + 1].a = (unsigned long)(xcnt / 2 == i) ? 1 : grid->a;
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

	MakeVertexBuffer(GizmoDatas, GizmoMeshPath);

	isActive = true;
}

