#pragma once
#include "../ForestPearlEngine/Define/FPMath.h"

struct FTransform
{
	FPVector3 Location;
	FPVector3 Rotation;
	FPQuaternion QuaternionRotation;
	FPVector3 Scale;

	FTransform()
	{
		Location = { 0.0f, 0.0f, 0.0f };
		Rotation = { 0.0f, 0.0f, 0.0f };
		QuaternionRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		Scale	 = { 1.0f, 1.0f, 1.0f };
	}
};