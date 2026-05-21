#pragma once
#include "Object.h"

class FPActor : public FPObject
{
	struct FPTransform
	{
		float x;
		float y;
		float z;
	};

	public :
		FPTransform Transform;

		virtual void BeginPlay() override = 0;
		virtual void Tick() override = 0;

		virtual ~FPActor() = default;
};