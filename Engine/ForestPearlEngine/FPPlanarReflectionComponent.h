#pragma once
#include "FPSceneCaptureComponent.h"
#include "FPPlanarReflectionList.h"

class FPPlanarReflectionComponent : public FPSceneCaptureComponent
{
	protected:
		FPVector4 Plane;
		FPMatrix ReflectionMatrix;

		PlanarReflectionItem* PlanarReflectionItem = nullptr;

		bool Active = true;

		virtual void CalculateTransformBranch() override;

		//현재 위치 / Forward 방향을 기준으로 Plane, ReflectionMatrix 갱신
		void UpdatePlaneAndReflectionMatrix();

	public:
		FPPlanarReflectionComponent(FPActor* Owner);
		~FPPlanarReflectionComponent();

		void RegistSceneCaptureComponent() override;
		void SetActive(bool Active) override;
		void SetRenderTarget(std::string& RenderTarget) override;
		void SetDepthStencil(std::string& DepthStencil) override;
		void SetCaptureEveryFrame(bool Active) override;
		void CaptureScene() override;

		void Tick() override;

		//HiddenActors 등록 함수
		void AddHiddenActor(FPActor* Acotr) override;
		void RemoveHiddenAcotr(FPActor* Actor) override;
		void EmptyHiddenActor() override;
};
