#pragma once
#include "FPSceneCaptureComponent.h"
#include "FPSceneCaptureList.h"

class FPSceneCaptureComponent2D : public FPSceneCaptureComponent
{
	protected:

		SceneCaptureItem* SceneCaptureItem = nullptr;

		//기본 Camera 속성
		
		//투영 변환 용
		float Fov = DegToRad(45.0f);							//시야각			
		float Aspect = 960.0f / 600.0f;							//가로:세로 비율
		float Zn = 1.0f;										//근평면 거리
		float Zf = 300.0f;										//원평면 거리
		FPProjectionMode ProjectionMode = Perspective;		
		float OrthoWidth = 100.0f;								
		FPMatrix ViewMatrix;									//View Matrix
		FPMatrix ProjectionMatrix;								//Projection Matrix
		bool Active = true;										//카메라 사용 여부

	public:
		FPSceneCaptureComponent2D(FPActor* Owner);
		~FPSceneCaptureComponent2D();

		void RegistSceneCaptureComponent() override;
		void SetActive(bool Active) override;
		void SetRenderTarget(std::string& RenderTarget) override;
		void SetDepthStencil(std::string& DepthStencil) override;
		void SetCaptureEveryFrame(bool Active) override;
		void CaptureScene() override;

		void Tick() override;

		//투영 변환 속성 제어 함수
		virtual void SetFov(float Fov);
		virtual void SetAspect(float Aspect);
		virtual void SetZn(float Zn);
		virtual void SetZf(float Zf);
		virtual void SetProjectionMode(FPProjectionMode Mode);
		virtual void SetOrthoWidth(float OrthoWidth);
		virtual FPProjectionMode GetProjectionMode() { return ProjectionMode; }

		//HiddenActors 운용 함수
		void AddHiddenActor(FPActor* Acotr) override;
		void RemoveHiddenAcotr(FPActor* Actor) override;
		void EmptyHiddenActor() override;

	protected:
		void RecalculateProjectionMatrix();
};