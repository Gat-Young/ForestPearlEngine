#pragma once
#include "FPGizmoRenderList.h"
#include "FPPrimitiveComponent.h"
#include "FPMaterial.h"
#include <iostream>
#include <vector>

struct GIZMO_VERTEX {
	float		x, y, z;
	float		r, g, b, a;
};

class GizmoComponent : FPPrimitiveComponent
{
	protected:
		std::string GizmoMeshPath;

		//값이 클수록 먼저 그려짐(작을 수록 앞으로 그려짐)
		int Priority = 0;
		bool isActive = true;

		FPMaterialInterface* Material = nullptr;

		GizmoRenderItem* RenderItem = nullptr;
		
		void MakeVertexBuffer(std::vector<GIZMO_VERTEX>& GizmoMesh, const std::string& GizmoMeshPath);
		void RegistGizmoRenderList();

	public:
		GizmoComponent(FPActor* Owner, const std::string& GizmoMeshPath);
		~GizmoComponent();

		void SetActive(bool Active) { this->isActive = Active; }
		void SetPriority(int Prio) { Priority = Prio; };
};