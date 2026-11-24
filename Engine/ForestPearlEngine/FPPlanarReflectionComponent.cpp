#include "FPPlanarReflectionComponent.h"
#include "FPGameInstance.h"
#include <algorithm>

//Owner 전달 생성자 (모든 클래스 생성자에서 전달전용함수를 쓸 수 없어 파생 생성자에서 직접 호출)
FPPlanarReflectionComponent::FPPlanarReflectionComponent(FPActor* Owner) : FPSceneCaptureComponent(Owner)
{
	//BackBuffer가 아닌 Planar Reflection 전용 RenderTarget/DepthStencil 사용
	this->RenderTarget = "DefaultPlanarReflectionRenderTarget";
	this->DepthStencil = "DefaultPlanarReflectionDepthStencil";

	//생성 시점의 현재 위치와 Forward 방향을 기준으로 Plane, ReflectionMatrix 생성
	UpdatePlaneAndReflectionMatrix();

	RegistSceneCaptureComponent();
}

FPPlanarReflectionComponent::~FPPlanarReflectionComponent()
{
	FPPlanarReflectionList* PlanarReflectionList = static_cast<FPPlanarReflectionList*>(FPGameInstance::Get().GetPlanarReflectionList());
	PlanarReflectionList->UnregistRenderList(PlanarReflectionItem);
}

//SceneCaptureComponent를 등록
void FPPlanarReflectionComponent::RegistSceneCaptureComponent()
{
	FPPlanarReflectionList* PlanarReflectionList = static_cast<FPPlanarReflectionList*>(FPGameInstance::Get().GetPlanarReflectionList());
	PlanarReflectionItem = PlanarReflectionList->RegistRenderList();

	PlanarReflectionItem->Location = &(this->WorldTransform.LocationMatrix);
	PlanarReflectionItem->Rotation = &(this->WorldTransform.RotationMatrix);
	PlanarReflectionItem->Scale = &(this->WorldTransform.ScaleMatrix);

	PlanarReflectionItem->Active = &(this->Active);
	PlanarReflectionItem->RenderTarget = &(this->RenderTarget);
	PlanarReflectionItem->DepthStencil = &(this->DepthStencil);
	PlanarReflectionItem->CaptureEveryFrame = &(this->CaptureEveryFrame);
	PlanarReflectionItem->CaptureThisFrame = &(this->CaptureThisFrame);
	PlanarReflectionItem->HiddenActors = &(this->HiddenActors);

	PlanarReflectionItem->Plane = &(this->Plane);
	PlanarReflectionItem->ReflectionMatrix = &(this->ReflectionMatrix);
}

void FPPlanarReflectionComponent::SetActive(bool Active)
{
	this->Active = Active;
}

void FPPlanarReflectionComponent::SetRenderTarget(std::string& RenderTarget)
{
	this->RenderTarget = RenderTarget;
}

void FPPlanarReflectionComponent::SetDepthStencil(std::string& DepthStencil)
{
	this->DepthStencil = DepthStencil;
}

void FPPlanarReflectionComponent::SetCaptureEveryFrame(bool Active)
{
	this->CaptureEveryFrame = Active;
	this->CaptureThisFrame = this->CaptureEveryFrame;
}

//호출 시 다음 프레임에 캡처
void FPPlanarReflectionComponent::CaptureScene()
{
	this->CaptureThisFrame = true;
}

//Tick에서 Transform 계산 분기 (필요에 따라 재계산 방식이 다른 컴포넌트들이 분기를 오버라이드)
void FPPlanarReflectionComponent::CalculateTransformBranch()
{
	__super::CalculateTransformBranch();

	//Transform이 바뀌었으니 Plane과 ReflectionMatrix도 갱신
	UpdatePlaneAndReflectionMatrix();
}

//현재 위치(Location)와 Forward 방향을 기준으로 Plane / ReflectionMatrix 생성
void FPPlanarReflectionComponent::UpdatePlaneAndReflectionMatrix()
{
	Plane = PlaneFromPointNormal(GetComponentLocation(), GetForwardVector());
	ReflectionMatrix = MatrixReflect(Plane);
}

void FPPlanarReflectionComponent::Tick()
{
	__super::Tick();
}

//HiddenActors 등록 함수
void FPPlanarReflectionComponent::AddHiddenActor(FPActor* Acotr)
{
	HiddenActors.push_back(Acotr->GetName());
}

void FPPlanarReflectionComponent::RemoveHiddenAcotr(FPActor* Actor)
{
	auto RawIt = std::find(HiddenActors.begin(), HiddenActors.end(), Actor->GetName());

	if (RawIt != HiddenActors.end())
	{
		HiddenActors.erase(RawIt);
	}
}

void FPPlanarReflectionComponent::EmptyHiddenActor()
{
	HiddenActors.clear();
}
