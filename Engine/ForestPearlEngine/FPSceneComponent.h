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
		void SetupAttachment(FPSceneComponent* Parent);
		void DetachFromComponent();

		//임시로 사용
		void Tick() override;
};