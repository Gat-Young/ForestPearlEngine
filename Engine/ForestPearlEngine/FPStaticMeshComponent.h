#pragma once
#include "FPMeshComponent.h"

class FPStaticMesh;

class FPStaticMeshComponent : public FPMeshComponent
{
	private:
		FPStaticMesh* StaticMesh;

	public:
		FPStaticMeshComponent(FPActor* Owner, std::string StaticMeshName);
		~FPStaticMeshComponent();

		//StaticMesh 등록 함수
		void SetStaticMesh(std::string StaticMeshName);

		//RenderItem 등록 함수 Override
		void SetRenderItemData() override;

		//Socket Transform 가져오기
		FTransform GetSocketTransform(const std::string& SocketName) const override;

};