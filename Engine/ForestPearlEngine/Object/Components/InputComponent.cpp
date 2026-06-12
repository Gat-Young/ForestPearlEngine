#include "InputComponent.h"
#include "InputAction.h"
#include "../../Systems/InputSystem.h"
#include <iostream>

FPInputComponent::FPInputComponent()
{

	IA = new FPInputAction();
	IMC = new FPInputMappingContext();
}

FPInputComponent::~FPInputComponent()
{
	delete IA;
	delete IMC;
}

void FPInputComponent::AddMappingKey(USHORT VKey, FMappingInfo MappingInfo)
{
	//std::cout << "AddMappingKey Begin\n";
	IMC->AddMappingKey(VKey, MappingInfo);
}

//void FPInputComponent::RemoveMappingKey(USHORT VKey)
//{
//	IMC->RemoveMappingKey(VKey);
//}

void FPInputComponent::ProcessInputTick()
{
	std::queue<std::pair<USHORT, EKeyState>>& InputQueue = FPInputSystem::GetInputSystem().GetInputQueue();

	int size = InputQueue.size();
	while (size>0)
	{
		std::pair<USHORT, EKeyState> KeyEvent = InputQueue.front();
		InputQueue.pop();
		std::cout << KeyEvent.first << " : " << KeyEvent.second << "\n";
		if (!ProcessKeyEvent(KeyEvent.first, KeyEvent.second))
			InputQueue.push(KeyEvent);
		size--;
	}
}

bool FPInputComponent::ProcessKeyEvent(USHORT VKey, EKeyState KeyState)
{
	if (IMC == nullptr)
	{
		std::cout << "IMC null\n";
		return FALSE;
	}
	if (IA == nullptr)
	{
		std::cout << "IA null\n";
		return FALSE;
	}

	if ((KeyState != CallKeyState) && (CallKeyState != EKeyState::Pressed || KeyState != EKeyState::Down))
	{
		return FALSE;
	}

	FMappingInfo MappingInfo;

	bool SearchResult = IMC->SearchMappingInfo(VKey, MappingInfo);

	if (SearchResult == false)
	{
		//std::cout << "SearchResult false\n";
		return FALSE;
	}

	FPVector2 InputValue;
	float Value = 1.0;

	if (MappingInfo.bIsPositive != ENegative::Positive)
	{
		Value *= -1;
	}

	switch (MappingInfo.Swizzle)
	{
	case ESwizzle::XYZ :
		InputValue.x = Value;
		break;
	case ESwizzle::YZX :
		InputValue.y = Value;
		break;
	default:
		InputValue.x = Value;
		break;
	//case ESwizzle::ZXY :
	//	InputValue.z = Value;
	//	break;
	}

	if (!BindFuncPtr)
	{
		std::cout << "BindFuncPtr is NULL!\n";
		return FALSE;
	}

	BindFuncPtr(InputValue);

	return TRUE;
}