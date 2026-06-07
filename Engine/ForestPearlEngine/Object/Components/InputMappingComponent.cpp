#include "InputMappingComponent.h"
#include "InputAction.h"
#include <iostream>

void FPInputMappingComponent::OnKeyStateChanged(USHORT VKey, EKeyState KeyState)
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
	FInputValue InputValue;
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
	else if (MappingInfo.ModifyInfo & (0b1 << 1))
	{
		InputValue.z = Value;
	}

	MappingInfo.InputAction->OnInputAction(KeyState, InputValue);
}

void FPInputMappingComponent::AddMappingKey(USHORT VKey, FMappingInfo MappingInfo)
{
	//std::cout << "AddMappingKey Begin\n";
	MappingKeys.insert({ VKey, MappingInfo });
}

void FPInputMappingComponent::RemoveMappingKey(USHORT VKey)
{
	MappingKeys.erase(VKey);
}
