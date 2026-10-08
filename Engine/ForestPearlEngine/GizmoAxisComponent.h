#pragma once
#include "GizmoComponent.h"


struct GIZMO_AXISINFO {
	float length;		//각 방향축의 길이.
	float scale;		//비례 스케일
	float r, g, b, a;
	int scnX, scnY;		//화면 크기(Viewport 미사용시 적용)
	unsigned long res[20];

	GIZMO_AXISINFO(float len = 5.0f, float s = 1.0f) :length(len), scale(s), scnX(0), scnY(0) {}
};

class GizmoAxisComponent : public GizmoComponent
{
	public:
		GizmoAxisComponent(FPActor* Owner, std::string GizmoMeshPath);
		~GizmoAxisComponent();
		void MakeAxis(GIZMO_AXISINFO* axis);
};