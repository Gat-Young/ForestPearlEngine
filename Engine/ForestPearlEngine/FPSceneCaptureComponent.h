#pragma once
#include "FPSceneComponent.h"
#include <vector>

class FPActor;

class FPSceneCaptureComponent : public FPSceneComponent
{
	protected:

		std::string RenderTarget = "BackbufferRenderTarget";								//사용하는 RenderTarget 이름
		std::string DepthStencil = "BackBufferDepthStencil";								//사용하는 DepthStencil 이름

		//SceneCapture 속성
		bool CaptureEveryFrame = true;
		//Capture Scene 용 체크
		bool CaptureThisFrame = true;							//RenderList 생성시 확인후 False로 변경 해 줄 것

		//해당 Actor들은 RenderTarget 캡처시 제외 함
		std::vector<std::string> HiddenActors;					//은닉 Actor의 목록

	public:
		FPSceneCaptureComponent(FPActor* Owner);
		~FPSceneCaptureComponent();
		virtual void RegistSceneCaptureComponent() = 0;
		virtual void SetActive(bool Active) = 0;
		virtual void SetRenderTarget(std::string& RenderTarget) = 0;
		virtual void SetDepthStencil(std::string& DepthStencil) = 0;
		virtual void SetCaptureEveryFrame(bool Active) = 0;
		virtual void CaptureScene() = 0;

		//투영 변환 속성 제어 함수
		virtual void SetFov(float Fov) = 0;
		virtual void SetAspect(float Aspect) = 0;
		virtual void SetZn(float Zn) = 0;
		virtual void SetZf(float Zf) = 0;
		
		//HiddenActors 운용 함수
		virtual void AddHiddenActor(FPActor* Acotr) = 0;
		virtual void RemoveHiddenAcotr(FPActor* Actor) = 0;
		virtual void EmptyHiddenActor() = 0;

		std::string GetRenderTarget() { return RenderTarget; };
		std::string GetDepthStencil() { return DepthStencil; };
};