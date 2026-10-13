#pragma once
#include "FPPrimitiveComponent.h"
#include "FPMaterial.h"


class FPGizmoNormalLineComponent: public FPPrimitiveComponent
{
	protected:
		std::string MeshPath;

		bool isActive = true;

		float NormalLength = 1.0f;
		FPMaterialInterface* Material = nullptr;

		NormalRenderItem* RenderItem = nullptr;

		void RegistNormalRenderList();

	public:
		FPGizmoNormalLineComponent(FPActor* Owner, std::string& StaticMeshPath);
		~FPGizmoNormalLineComponent();

		void SetMeshPath(const std::string& StaticMeshPath);

		void SetActive(bool Active) { this->isActive = Active; }
		void SetNormalLength(float Length) { NormalLength = Length; }
};