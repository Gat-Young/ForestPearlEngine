#include "InputMappingContext.h"
#include "InputAction.h"
#include "../../Define/FPMath.h"
#include <iostream>

void FPInputMappingContext::OnKeyStateChanged(USHORT VKey, EKeyState KeyState)
{
	//std::cout << "OnKeyStateChanged Begin\n";
	auto it = MappingKeys.find(VKey);
	if (it == MappingKeys.end())
	{
		//std::cout << "MappingKey Not Detected!\n";
		return;
	}

	//std::cout << "MappingKey Detected!\n";

	FMappingInfo MappingInfo = it->second;
	FPVector2 InputValue;
	float Value = 1.0;

	if (MappingInfo.ModifyInfo & 0b1)
		Value *= -1;

	if (MappingInfo.ModifyInfo & (0b1 << 3))
	{
		InputValue.x = Value;
	}
	else if (MappingInfo.ModifyInfo & (0b1 << 2))
	{
		InputValue.y = Value;
	}

	MappingInfo.InputAction->OnInputAction(KeyState, InputValue);
}

void FPInputMappingContext::AddMappingKey(USHORT VKey, FMappingInfo MappingInfo)
{
	//std::cout << "AddMappingKey Begin\n";
	MappingKeys.insert({ VKey, MappingInfo });
}

void FPInputMappingContext::RemoveMappingKey(USHORT VKey)
{
	MappingKeys.erase(VKey);
}
