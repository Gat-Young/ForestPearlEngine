#include "GizmoAxisComponent.h"

GizmoAxisComponent::GizmoAxisComponent(FPActor* Owner, std::string GizmoMeshPath) : GizmoComponent(Owner, GizmoMeshPath)
{
	GIZMO_AXISINFO axis;
	MakeAxis(&axis);
	RegistGizmoRenderList();
}

GizmoAxisComponent::~GizmoAxisComponent()
{
}

void GizmoAxisComponent::MakeAxis(GIZMO_AXISINFO* axis)
{
	std::vector<GIZMO_VERTEX> GizmoDatas;

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

	isActive = true;

	MakeVertexBuffer(GizmoDatas, GizmoMeshPath);
}