#include "FPSceneCaptureComponent2D.h"
#include "FPGameInstance.h"
#include <algorithm>


//SceneCaptureComponent를 등록
void FPSceneCaptureComponent2D::RegistSceneCaptureComponent()
{
	FPSceneCaptureList* SceneCaptureList = static_cast<FPSceneCaptureList*>(FPGameInstance::Get().GetSceneCaptureList());
	SceneCaptureItem = SceneCaptureList->RegistRenderList();

	SceneCaptureItem->Location = &(this->WorldTransform.LocationMatrix);
	SceneCaptureItem->Rotation = &(this->WorldTransform.RotationMatrix);
	SceneCaptureItem->Scale = &(this->WorldTransform.ScaleMatrix);

	SceneCaptureItem->View = &(this->ViewMatrix);
	SceneCaptureItem->Projection = &(this->ProjectionMatrix);

	SceneCaptureItem->Active = &(this->Active);
	SceneCaptureItem->RenderTarget = &(this->RenderTarget);
	SceneCaptureItem->CaptureEveryFrame = &(this->CaptureEveryFrame);
	SceneCaptureItem->CaptureThisFrame = &(this->CaptureThisFrame);
	SceneCaptureItem->HiddenActors = &(this->HiddenActors);
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

void FPSceneCaptureComponent2D::SetFov(float Fov)
{
	this->Fov = DegToRad(Fov);
}

void FPSceneCaptureComponent2D::SetAspect(float Aspect)
{
	this->Aspect = Aspect;
}

void FPSceneCaptureComponent2D::SetZn(float Zn)
{
	this->Zn = Zn;
}

void FPSceneCaptureComponent2D::SetZf(float Zf)
{
	this->Zf = Zf;
}

//호출 시 현재 프레임을 캡처
void FPSceneCaptureComponent2D::CaptureScene()
{
	this->CaptureThisFrame = true;
}

//HiddenActors 운용 함수
void FPSceneCaptureComponent2D::AddHiddenActor(FPActor* Acotr)
{
	HiddenActors.push_back(Acotr->GetName());
}

void FPSceneCaptureComponent2D::RemoveHiddenAcotr(FPActor* Actor)
{
	auto RawIt = std::find(HiddenActors.begin(), HiddenActors.end(), Actor->GetName());

	if (RawIt != HiddenActors.end())
	{
		HiddenActors.erase(RawIt);
	}
}

void FPSceneCaptureComponent2D::EmptyHiddenActor()
{
	HiddenActors.clear();
}
