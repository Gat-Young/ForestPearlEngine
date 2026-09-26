#pragma once
#include "../ForestPearlEngine/Define/FPMath.h"

struct FTransform
{
	FPVector3 Location;
	FPVector3 Rotation;
	FPQuaternion QuaternionRotation;
	FPVector3 Scale;

	FPMatrix LocationMatrix;
	FPMatrix RotationMatrix;
	FPMatrix ScaleMatrix;

	FTransform()
	{
		Location = { 0.0f, 0.0f, 0.0f };
		Rotation = { 0.0f, 0.0f, 0.0f };
		QuaternionRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		Scale	 = { 1.0f, 1.0f, 1.0f };
	}

	FPVector3 Right()
	{
		FPVector3 Right(RotationMatrix.Matrix.r[0]);
		return Right;
	}

	FPVector3 Up()
	{
		FPVector3 Up(RotationMatrix.Matrix.r[1]);
		return Up;
	}

	FPVector3 Foraward()
	{
		FPVector3 Forward(RotationMatrix.Matrix.r[2]);
		return Forward;
	}
};