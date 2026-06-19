#pragma once
#include "Object.h"

class FPActor : public FPObject
{
	protected:
		class FPSceneComponent* RootComponent = nullptr;

	public :
		virtual void Initialize() override = 0;
		virtual void BeginPlay() override = 0;
		virtual void Tick() override = 0;

		virtual ~FPActor() = default;

		virtual FPWorld* GetWorld() override final{ return Outer->GetWorld(); }

		bool AttachToComponent(FPSceneComponent* Parent);
		bool AttachToActor(FPActor* Parent);
		bool DetachFromActor();
		bool SetRootComponent(FPSceneComponent* Component);
};