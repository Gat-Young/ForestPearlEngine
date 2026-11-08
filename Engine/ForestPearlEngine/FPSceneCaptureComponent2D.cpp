#include "FPSceneCaptureComponent2D.h"


//SceneCaptureComponent를 등록
void FPSceneCaptureComponent2D::RegistSceneCaptureComponent()
{

}

void FPSceneCaptureComponent2D::SetActive(bool Active)
{
	this->Active = Active;
}

void FPSceneCaptureComponent2D::SetRenderTarget(std::string& RenderTarget)
{
	this->RenderTarget = RenderTarget;
}

void FPSceneCaptureComponent2D::SetCaptureEveryFrame(bool Active)
{
	this->CaptureEveryFrame = Active;
	this->CaptureThisFrame = this->CaptureEveryFrame;
}

//호출 시 현재 프레임을 캡처
void FPSceneCaptureComponent2D::CaptureScene()
{
	this->CaptureThisFrame = true;
}
