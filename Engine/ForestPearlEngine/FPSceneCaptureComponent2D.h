#pragma once
#include "FPSceneCaptureComponent.h"

class FPSceneCaptureComponent2D : public FPSceneCaptureComponent
{
	public:
		void RegistSceneCaptureComponent() override;
		void SetActive(bool Active) override;
		void SetRenderTarget(std::string& RenderTarget) override;
		void SetDepthStencil(std::string& DepthStencil) override;
		void SetCaptureEveryFrame(bool Active) override;
		void CaptureScene() override;

		//투영 변환 속성 제어 함수
		void SetFov(float Fov) override;
		void SetAspect(float Aspect) override;
		void SetZn(float Zn) override;
		void SetZf(float Zf) override;

		//HiddenActors 운용 함수
		void AddHiddenActor(FPActor* Acotr) override;
		void RemoveHiddenAcotr(FPActor* Actor) override;
		void EmptyHiddenActor() override;
};