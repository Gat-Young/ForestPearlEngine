#pragma once

#include "../../Systems/KeyStateEnum.h"

struct FInputValue
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};

class FPInputAction
{
public:
	void OnInputAction(EKeyState KeyState, FInputValue KeyValue);
};