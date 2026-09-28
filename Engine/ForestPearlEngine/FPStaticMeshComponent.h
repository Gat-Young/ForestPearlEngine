#pragma once
#include "FPMeshComponent.h"

class FPStaticMesh;

class FPStaticMeshComponent : public FPMeshComponent
{
	private:
		FPStaticMesh* StaticMesh;

	public:
		FPStaticMeshComponent(FPActor* Owner, std::string MeshPath);
		~FPStaticMeshComponent();

		//RenderItem 등록 함수 Override
		void SetRenderItemData() override;
};