#pragma once
#include "../ForestPearlEngine/Define/FPMath.h"

struct FTransform
{
	FPVector3 Location;
	FPVector3 Rotation;
	FPVector3 Scale;

	FTransform()
	{
		Location = { 0.0f, 0.0f, 0.0f };
		Rotation = { 0.0f, 0.0f, 0.0f };
		Scale	 = { 0.0f, 0.0f, 0.0f };
	}
};