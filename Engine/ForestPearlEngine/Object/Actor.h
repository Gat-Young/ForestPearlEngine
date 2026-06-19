#pragma once
#include "Object.h"

class FPActor : public FPObject
{
	protected:
		class FPSceneComponent* RootComponent = nullptr;

	public :
		virtual void Initialize() override = 0;
		virtual void BeginPlay() override = 0;

		//임시로 Component Tick 수행 반드시 __super로 실행 시킬 것
		virtual void Tick() override;

		virtual ~FPActor() = default;

		virtual FPWorld* GetWorld() override final{ return Outer->GetWorld(); }

		bool AttachToComponent(FPSceneComponent* Parent);
		bool AttachToActor(FPActor* Parent);
		bool DetachFromActor();
		bool SetRootComponent(FPSceneComponent* Component);

		//이동 World
		void SetActorLocation(struct FPVector3 Location);
		void SetActorRotation(struct FPVector3 Rotation);
		void SetActorScale3D(struct FPVector3 Scale);

		//Transform 가져오기 World
		struct FTransform GetActorTransform();
		struct FPVector3 GetActorLocation();
		struct FPVector3 GetActorRotation();
		struct FPVector3 GetActorScale3D();
};