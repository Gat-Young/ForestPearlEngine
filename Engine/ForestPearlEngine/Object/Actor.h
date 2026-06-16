#pragma once
#include "Object.h"

class FPActor : public FPObject
{
	protected:


	public :
		virtual void Initialize() override = 0;
		virtual void BeginPlay() override = 0;
		virtual void Tick() override = 0;

		virtual ~FPActor() = default;

		virtual FPWorld* GetWorld() override final{ return Outer->GetWorld(); }
};