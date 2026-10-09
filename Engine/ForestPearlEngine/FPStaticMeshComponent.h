#pragma once
#include "FPMeshComponent.h"

class FPStaticMesh;
class FPGizmoNormalLineComponent;

class FPStaticMeshComponent : public FPMeshComponent
{
	private:
		FPStaticMesh* StaticMesh;
		FPGizmoNormalLineComponent* GizmoNormalLine;

	public:
		FPStaticMeshComponent(FPActor* Owner, std::string StaticMeshName);
		~FPStaticMeshComponent();

		void SetActive(bool Active) override;
		void SetPriority(int Prio) override;

		//StaticMesh 등록 함수
		void SetStaticMesh(std::string StaticMeshName);

		//RenderItem 등록 함수 Override
		void SetRenderItemData() override;

		//Socket Transform 가져오기
		FTransform GetSocketTransform(const std::string& SocketName) const override;

};