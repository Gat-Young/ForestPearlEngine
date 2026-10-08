#pragma once
#include "FPMeshRenderList.h"
#include "FPPrimitiveComponent.h"
#include <string>

class FPMaterialInterface;

class FPMeshComponent : public FPPrimitiveComponent
{
	protected:
		std::string MeshName;

		int Priority = 0;
		bool isActive = true;
		FPMaterialInterface* Material = nullptr;

		RenderItem* RenderItem = nullptr;

		void RegistMeshRenderList();
		virtual void SetRenderItemData() = 0;

	public:
		FPMeshComponent(FPActor* Owner, std::string MesName);

		~FPMeshComponent();

		void SetActive(bool Active) { this->isActive = Active; }
		void SetPriority(int Prio) { Priority = Prio; };
		void SetMaterial(FPMaterialInterface* Material);
};