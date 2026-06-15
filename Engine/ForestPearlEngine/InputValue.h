#pragma once

struct FInputValue
{
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	bool bBool = true;
	float Float = 0.0f;

	FInputValue operator* (int value)
	{ 
		X = X * value;
		Y = Y * value;
		Z = Z * value;
		Float = Float * value;

		return *this;
	}
};