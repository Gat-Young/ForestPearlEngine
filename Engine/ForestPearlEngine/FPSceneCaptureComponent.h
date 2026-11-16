#pragma once
#include "FPSceneComponent.h"
#include "FPSceneCaptureList.h"
#include <vector>

class FPActor;

class FPSceneCaptureComponent : public FPSceneComponent
{
	protected:

		SceneCaptureItem* SceneCaptureItem = nullptr;

		//기본 Camera 속성
		
		//투영 변환 용
		float Fov = DegToRad(45.0f);							//시야각			
		float Aspect = 960.0f / 600.0f;							//가로:세로 비율
		float Zn = 1.0f;										//근평면 거리
		float Zf = 300.0f;										//원평면 거리
		FPMatrix ViewMatrix;									//View Matrix
		FPMatrix ProjectionMatrix;								//Projection Matrix
		bool Active = true;										//카메라 사용 여부

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