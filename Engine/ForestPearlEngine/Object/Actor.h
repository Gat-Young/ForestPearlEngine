#pragma once
#include "Object.h"
#include "../Define/FPMath.h"
#include <string>

class FPSceneComponent;

class FPActor : public FPObject
{
	protected:
		std::string ActorName = "";
		FPSceneComponent* RootComponent = nullptr;

		//Actor Bounds (RootComponent + 하위 모든 SceneComponent Bound를 합침)
		FPBoxSphereBounds Bounds{};

		void UpdateBounds();
		void CombineChildBounds(FPSceneComponent* Component);

	public :
		FPActor();
		virtual void Initialize() = 0;
		virtual void BeginPlay() = 0;

		//임시로 Component Tick 수행 반드시 __super로 실행 시킬 것
		virtual void Tick();

		virtual ~FPActor() = default;

		virtual FPWorld* GetWorld() override final{ return Outer->GetWorld(); }

		bool AttachToComponent(FPSceneComponent* Parent, std::string SocketName = "");
		bool AttachToActor(FPActor* Parent, std::string SocketName = "");
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

		//축 벡터 (RootComponent 기준, DX 기준 : Right +X, Up +Y, Forward +Z)
		struct FPVector3 GetActorForwardVector();
		struct FPVector3 GetActorRightVector();
		struct FPVector3 GetActorUpVector();

		//가장 최신의 Bounds 정보를 가져옴 (즉시 재계산)
		FPBoxSphereBounds GetBounds();

		//생성된 FPActor의 이름을 설정
		void SetActorName(std::string Name) { ActorName = Name; }
		//이름 가져오기
		std::string GetName() { return ActorName; }
};