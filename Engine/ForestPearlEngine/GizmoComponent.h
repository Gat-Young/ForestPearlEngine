#pragma once
#include "FPMeshRenderList.h"
#include "FPPrimitiveComponent.h"
#include "FPMaterial.h"
#include <vector>

struct GIZMO_VERTEX {
	float		x, y, z;
	float		r, g, b, a;
};

struct GRIDINFO {
	int width;
	int height;
	float scale;
	float r, g, b, a;
	unsigned long res[20];

	GRIDINFO(int w = 256, int h = 256, float s = 1.0f, float r = 0.3f , float g = 0.3f, float b=0.3f, float a = 1.0f )
		:width(w), height(h), scale(s), r(r), g(g), b(b), a(a) {
	}
};

struct GIZMO_AXISINFO {
	//int width; 
	//int height;
	float length;		//각 방향축의 길이.
	float scale;		//비례 스케일
	float r, g, b, a;
	int scnX, scnY;		//화면 크기(Viewport 미사용시 적용)
	unsigned long res[20];

	GIZMO_AXISINFO(float len = 5.0f, float s = 1.0f) :length(len), scale(s), scnX(0), scnY(0) {}
};

class GizmoComponent : FPPrimitiveComponent
{
	private:
		std::vector<GIZMO_VERTEX> GizmoDatas;

		//값이 클수록 먼저 그려짐(작을 수록 앞으로 그려짐)
		int Priority = 0;
		bool isActive = true;

		FPMaterialInterface* Material = nullptr;

		RenderItem* RenderItem = nullptr;
		
		void* MakeVertexBuffer(std::vector<GIZMO_VERTEX> GizmoMesh);

	public:
		GizmoComponent(FPActor* Owner);
		~GizmoComponent();

		void MakeGrid(GRIDINFO* grid);
		void MakeAxis(GIZMO_AXISINFO* axis);

		void RegistGizmoRenderList();

		void SetMeshFill(bool State) { isFill = State; };
		void SetMeshCull(bool State) { isCull = State; };
		void SetActive(bool Active) { this->isActive = Active; }
		void SetTopology(Topology State) { Topo = State; };
		void SetPriority(int Prio) { Priority = Prio; };
};