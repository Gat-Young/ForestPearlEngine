#include "InputComponent.h"
#include "InputAction.h"
#include "../../Systems/InputSystem.h"
#include <iostream>

FPInputComponent::FPInputComponent()
{
	IMC = new FPInputMappingContext();
}

FPInputComponent::~FPInputComponent()
{
	delete IMC;

	for (auto& pair : ActivatedIA) {
		delete pair.second;
	}
	ActivatedIA.clear();
}

void FPInputComponent::AddMappingKey(std::string IANAme, USHORT VKey, FModifyInfo MappingInfo)
{
	//std::cout << "AddMappingKey Begin\n";
	IMC->AddMappingKey(IANAme, VKey, MappingInfo);
}

//void FPInputComponent::RemoveMappingKey(USHORT VKey)
//{
//	IMC->RemoveMappingKey(VKey);
//}

void FPInputComponent::ProcessInputTick()
{
	std::queue<FKeyInputInfo>& InputQueue = FPInputSystem::GetInputSystem().GetInputQueue();

	while (InputQueue.size() != 0)
	{
		FKeyInputInfo KeyEvent = InputQueue.front();
		ProcessKeyEvent(KeyEvent);
		InputQueue.pop();
	}

	//int size = InputQueue.size();
	//while (size>0)
	//{
	//	std::pair<USHORT, EKeyState> KeyEvent = InputQueue.front();
	//	InputQueue.pop();
	//	//std::cout << KeyEvent.first << " : " << KeyEvent.second << "\n";
	//	if (!ProcessKeyEvent(KeyEvent.first, KeyEvent.second))
	//		InputQueue.push(KeyEvent);
	//	size--;
	//}
}

bool FPInputComponent::ProcessKeyEvent(FKeyInputInfo KeyInputInfo)
{
	if (IMC == nullptr)
	{
		std::cout << "IMC null\n";
		return FALSE;
	}

	std::string IAName;
	FModifyInfo ModifyInfo;
	bool SearchResult = IMC->SearchMappingInfo(KeyInputInfo.VKey, IAName, ModifyInfo);

	if (SearchResult == false)
	{
		//std::cout << "SearchResult false\n";
		return FALSE;
	}

	if (ActivatedIA.find(IAName) == ActivatedIA.end())
	{
		std::cout << "IA null\n";
		return FALSE;
	}

	std::vector<FBindInfo>& BindInfos = ActivatedIA[IAName]->GetBindInfos();

	for (FBindInfo BindInfo : BindInfos)
	{
		EKeyState CallKeyState = BindInfo.CallState;
		std::function<void(FInputValue)> BindFuncPtr = BindInfo.BindFuncPtr;

		if ((KeyInputInfo.KeyState != CallKeyState) && (CallKeyState != EKeyState::Pressed || KeyInputInfo.KeyState != EKeyState::Down))
		{
			continue;
		}

		FInputValue InputData = KeyInputInfo.InputValue;

		if (ModifyInfo.bIsPositive != ENegative::Positive)
		{
			InputData = InputData * -1;
		}

		float X, Y, Z = 0.0f;

		switch (ModifyInfo.Swizzle)
		{
		case ESwizzle::YZX:
			X = InputData.X;
			Y = InputData.Y;
			Z = InputData.Z;
			InputData.Y = X;
			InputData.Z = Y;
			InputData.X = Z;
			break;
		case ESwizzle::ZXY :
			X = InputData.X;
			Y = InputData.Y;
			Z = InputData.Z;
			InputData.Z = X;
			InputData.X = Y;
			InputData.Y = Z;
			break;
		default:
			break;
		}

		if (!BindFuncPtr)
		{
			std::cout << "BindFuncPtr is NULL!\n";
			return FALSE;
		}

		BindFuncPtr(InputData);
	}

	return TRUE;
}