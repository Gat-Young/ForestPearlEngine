#include "InputComponent.h"
#include "InputAction.h"
#include "../../FPGameInstance.h"

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
	FPInputSystem* InputSystem = static_cast<FPInputSystem*>(FPGameInstance::Get().GetInputSystem());
	std::queue<FKeyInputInfo>& InputQueue = InputSystem->GetInputQueue();

	while (InputQueue.size() != 0)
	{
		FKeyInputInfo KeyEvent = InputQueue.front();
		ProcessKeyEvent(KeyEvent);
		InputQueue.pop();
	}

	//for (USHORT Key : FPInputSystem::GetInputSystem().GetPressedKeys())
	//{
	//	if (GetAsyncKeyState(Key) & 0x8000)
	//	{
	//		FInputValue InputValue = { 1.0f, 0.0f, 0.0f, true, 1.0f };
	//		FKeyInputInfo PressedKeyEvent = { Key, EKeyState::Pressed, InputValue };
	//		ProcessKeyEvent(PressedKeyEvent);
	//	}
	//}
}

void FPInputComponent::Possess(void* Actor)
{
	this->PossessedActor = Actor;
}

void FPInputComponent::UnPossess()
{
	this->PossessedActor = nullptr;
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

	//std::cout << "VKey : " << KeyInputInfo.VKey << " KeyState : " << KeyInputInfo.KeyState << "\n";
	//std::cout << "SearchResult IAName : " << IAName << " KeyState : " << KeyInputInfo.KeyState << "\n";

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
		if (BindInfo.BindObj != PossessedActor) continue;

		EKeyState CallKeyState = BindInfo.CallState;
		std::function<void(FInputValue)> BindFuncPtr = BindInfo.BindFuncPtr;

		if (KeyInputInfo.KeyState != CallKeyState)
		{
			//std::cout << "Wrong State! : "<< CallKeyState << "\n";
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