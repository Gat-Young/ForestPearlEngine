#pragma once
#include "GizmoRenderList.h"
#include <vector>

#define COLOR_ARGB(a,r,g,b) \
   ((unsigned long)((((unsigned long)((a) * 255.0f) & 0xff) << 24) | \
                     (((unsigned long)((r) * 255.0f) & 0xff) << 16) | \
                     (((unsigned long)((g) * 255.0f) & 0xff) << 8)  | \
                     (((unsigned long)((b) * 255.0f) & 0xff))))

struct GIZMO_COLVTX {
	FPVector3		vPos;
	unsigned long	Color;
};

struct GRIDINFO {
	int width;
	int height;
	float scale;
	unsigned long color;
	unsigned long res[20];

	GRIDINFO(int w = 100, int h = 100, float s = 1.0f,
		unsigned long c = COLOR_ARGB(1.0f, 0.3f, 0.3f, 0.3f) )
		:width(w), height(h), scale(s), color(c) {
	}
};

struct GIZMO_AXISINFO {
	//int width; 
	//int height;
	float length;		//각 방향축의 길이.
	float scale;		//비례 스케일
	//unsigned long color;
	int scnX, scnY;		//화면 크기(Viewport 미사용시 적용)
	unsigned long res[20];

	GIZMO_AXISINFO(float len = 1.0f, float s = 1.0f) :length(len), scale(s), scnX(0), scnY(0) {}
};

class GizmoComponent
{
	private:
		std::vector<GIZMO_COLVTX> GizmoDatas;
		int VBIndex;
		int LineCount;
		bool Active;
		GizmoRenderItem* RenderItem = nullptr;

		int MakeVertexVuffer(std::vector<GIZMO_COLVTX> GizmoMesh);

	public:
		GizmoComponent() = default;
		~GizmoComponent();

		void MakeGrid(GRIDINFO* grid);
		void MakeAxis(GIZMO_AXISINFO* axis);

		void RegistGizmoRenderList(FPVector3* Position, FPVector3* Rotation, FPVector3* Scale);

		void SetActive(bool Active) { this->Active = Active; }
};