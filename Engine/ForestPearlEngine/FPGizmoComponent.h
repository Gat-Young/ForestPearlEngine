#pragma once
#include "FPGizmoRenderList.h"
#include "FPPrimitiveComponent.h"
#include "FPMaterial.h"
#include <iostream>
#include <vector>

struct FPGIZMO_VERTEX {
	float		x, y, z;
	float		r, g, b, a;
};

class FPGizmoComponent : public FPPrimitiveComponent
{
	protected:
		std::string GizmoClass;
		std::string GizmoMeshPath;

		//값이 클수록 먼저 그려짐(작을 수록 앞으로 그려짐)
		int Priority = 0;
		bool isActive = true;

		FPMaterialInterface* Material = nullptr;

		GizmoRenderItem* RenderItem = nullptr;
		
		void MakeVertexBuffer(std::vector<FPGIZMO_VERTEX>& GizmoMesh, const std::string& GizmoMeshPath);
		void RegistGizmoRenderList();

	public:
		FPGizmoComponent(FPActor* Owner, const std::string& GizmoClass ,const std::string& GizmoMeshPath);
		~FPGizmoComponent();

		virtual void SetGizmoMeshPath(const std::string& GizmoMeshPath) { this->GizmoMeshPath = GizmoMeshPath; }
		void SetActive(bool Active) { this->isActive = Active; }
		void SetPriority(int Prio) { Priority = Prio; };
};