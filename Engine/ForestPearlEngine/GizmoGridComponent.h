#pragma once
#include "GizmoComponent.h"

struct GIZMO_GRIDINFO {
	int width;
	int height;
	float scale;
	float r, g, b, a;
	unsigned long res[20];

	GIZMO_GRIDINFO(int w = 256, int h = 256, float s = 1.0f, float r = 0.3f, float g = 0.3f, float b = 0.3f, float a = 1.0f)
		:width(w), height(h), scale(s), r(r), g(g), b(b), a(a) {
	}
};

class GizmoGridComponent : public GizmoComponent
{
public:
	GizmoGridComponent(FPActor* Owner, std::string GizmoMeshPath);
	~GizmoGridComponent();
	void MakeGrid(GIZMO_GRIDINFO* axis);
};