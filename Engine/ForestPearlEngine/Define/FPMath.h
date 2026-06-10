#pragma once

struct FPVector4
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};

struct FPVector3
{
	float x = 0;
	float y = 0;
	float z = 0;
};

struct FPVector2
{
	float x = 0;
	float y = 0;

	explicit operator float() const { return x; }
	explicit operator bool() const { return x != 0.f || y != 0.f; }
};