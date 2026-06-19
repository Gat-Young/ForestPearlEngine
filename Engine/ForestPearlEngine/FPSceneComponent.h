#pragma once
#include "FPActorComponent.h"
#include "FTransform.h"

////////////////////////////////////////////////////////
//지오메트리 표현이 필요하지 않은 위치 기반 동작을 지원
//스피링 암, 카메라, 물리적 힘, 컨스트레인트, 오디오 등 포함
class FPSceneComponent : public FPActorComponent
{
	protected:
		FTransform WorldTransform;
		FTransform RelativeTransform;
		
		FPSceneComponent* ParentComponent = nullptr;
		std::vector<FPSceneComponent*> ChildComponent;


		void AttachChildCompont(FPSceneComponent* Child);
		void DetachChildComponet(FPSceneComponent* Child);

	public:
		FPSceneComponent(FPActor* Owner);
		void SetupAttachment(FPSceneComponent* Parent);
		void DetachFromComponent();

		//이동 시키기
		void SetRelativeLocation(FPVector3 Location);
		void SetRelativeRotation(FPVector3 Rotation);
		void SetRelativeScale3D(FPVector3 Scale);

		void SetWorldLocation(FPVector3 Location);
		void SetWorldRotation(FPVector3 Rotation);
		void SetWorldScale3D(FPVector3 Scale);

		//Transform 가져오기
		FTransform GetComponentTransform();
		FTransform GetRelativeTransform();
		//Relative
		FPVector3 GetRelativeLocation();
		FPVector3 GetRelativeRotation();
		FPVector3 GetRelativeScale3D();

		//World
		FPVector3 GetComponentLocation();
		FPVector3 GetComponentRotation();
		FPQuaternion GetComponentQuat();
		FPVector3 GetComponentScale();

		//임시로 사용
		void Tick() override;
};