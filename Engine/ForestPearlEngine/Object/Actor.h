#pragma once
#include "Object.h"
#include <vector>
#include "../FPLevel.h"

class FPActor : public FPObject
{

	protected:
		struct FPTransform
		{
			float x;
			float y;
			float z;
		};

	public :
		FPTransform Transform;

		virtual void Initialize() override = 0;
		virtual void BeginPlay() override = 0;
		virtual void Tick() override = 0;

		virtual ~FPActor() = default;

		virtual FPWorld* GetWorld() override final { return Outer->GetWorld(); };
};