#pragma once
#include <vector>
#include <functional>
#include "../../MCLOG.h"
#include "Windows.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "../../FPGameInstance.h"
#include "../../Systems/KeyStateEnum.h"
#include "../../Define/FPMath.h"
#include "../../Systems/InputSystem.h"
#include "../../InputValue.h"

class FPInputComponent
{
public:
	FPInputComponent(void* Controller);
	~FPInputComponent();

	FPInputAction& GetIA(std::string IAName) { return *ActivatedIA[IAName]; }
	FPInputMappingContext& GetIMC() { return *IMC; }

	void AddMappingKey(std::string IANAme, USHORT VKey, FModifyInfo MappingInfo);
	//void RemoveMappingKey(USHORT VKey);

	template<typename TObj>
	void BindMethod(std::string IAName, TObj* BindActor, EKeyState BindKeyState, void(TObj::* FuncPtr)(FInputValue))
	{
		if (ActivatedIA.find(IAName) == ActivatedIA.end())
		{
			FPInputAction* IA = new FPInputAction();
			ActivatedIA[IAName] = IA;

			if (BindKeyState == EKeyState::Pressed)
			{
				if (IMC == nullptr)
				{
					MCLOG(ErrorMC, "[FPInputComponent::BindMethod] IMC == nullptr");
				}
				else
				{
					FPInputSystem* InputSystem = static_cast<FPInputSystem*>(FPGameInstance::Get().GetInputSystem());
					IMC->GetMappedKeys(IAName, InputSystem->GetCheckPressedKeys());
					//std::cout << "[FPInputComponent::BindMethod] Bind Completed!\n";
				}
			}
		}

		FBindInfo BindInfo;

		BindInfo.CallState = BindKeyState;

		BindInfo.BindObj = BindActor;

		//std::cout << "BindActor ptr: " << (void*)BindActor << "\n";
		BindInfo.BindFuncPtr = [BindActor, FuncPtr](FInputValue val)
			{
				//std::cout << "Lambda called! BindActor: " << (void*)BindActor << "\n";
				(BindActor->*FuncPtr)(val);
			};

		ActivatedIA[IAName]->BindFunc(BindInfo);
	}

	void ProcessInputTick();

	void Possess(void* Pawn);
	void UnPossess();

private:
	bool ProcessKeyEvent(struct FKeyInputInfo KeyInputInfo);

private:
	FPInputMappingContext* IMC = nullptr;

	std::map<std::string, FPInputAction*> ActivatedIA;

	void* PossessedPawn = nullptr;
	void* PlayerController = nullptr;
	//std::set<USHORT> PressedKeys;
};

