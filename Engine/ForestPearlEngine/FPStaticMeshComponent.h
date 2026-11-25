#pragma once
#include "FPMeshComponent.h"

class FPStaticMesh;
class FPGizmoNormalLineComponent;

class FPStaticMeshComponent : public FPMeshComponent
{
	private:
		FPStaticMesh* StaticMesh;
		FPGizmoNormalLineComponent* GizmoNormalLine;
		int StencilRefValue = 0;
		bool bCustomDepthStencil = false;

	protected:
		//StaticMesh의 Bounds 정보를 가져와 Local Bounds로 사용
		FPBoxSphereBounds CalculateBounds() override;

	public:
		FPStaticMeshComponent(FPActor* Owner, std::string StaticMeshName);
		~FPStaticMeshComponent();

		void SetActive(bool Active) override;
		void SetPriority(int Prio) override;

		//StaticMesh 등록 함수
		void SetStaticMesh(std::string StaticMeshName);

		//Custom DepthStencil (CustomDepthStencilPass) 설정
		void SetStencilRefValue(int InStencilRefValue);
		int GetStencilRefValue() const;
		void SetCustomDepthStencil(bool bInCustomDepthStencil);
		bool GetCustomDepthStencil() const;

		//RenderItem 등록 함수 Override
		void SetRenderItemData() override;

		//Socket Transform 가져오기
		FTransform GetSocketTransform(const std::string& SocketName) const override;

};