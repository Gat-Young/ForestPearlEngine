#pragma once
#include "Object.h"
#include <string>

class FPSceneComponent;

class FPActor : public FPObject
{
	protected:
		std::string ActorName = "";
		FPSceneComponent* RootComponent = nullptr;

	public :
		FPActor();
		virtual void Initialize() = 0;
		virtual void BeginPlay() = 0;

		//임시로 Component Tick 수행 반드시 __super로 실행 시킬 것
		virtual void Tick();

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

		//생성된 FPActor의 이름을 설정
		void SetActorName(std::string Name) { ActorName = Name; }
		//이름 가져오기
		std::string GetName() { return ActorName; }
};