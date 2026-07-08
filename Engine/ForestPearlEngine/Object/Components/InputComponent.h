#pragma once
#include <vector>
#include <functional>
#include <set>
#include "InputMappingContext.h"
#include "InputAction.h"
#include "../../Systems/KeyStateEnum.h"
#include "../../Define/FPMath.h"
#include "Windows.h"
#include <iostream>
#include "../../InputValue.h"

class FPInputComponent
{
public:
	FPInputComponent();
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
					std::cout << "[FPInputComponent::BindMethod] IMC == nullptr\n";
				}
				else
				{
					IMC->GetMappedKeys(IAName, PressedKeys);
					//std::cout << "[FPInputComponent::BindMethod] Bind Completed!\n";
				}
			}
		}

		FBindInfo BindInfo;

		BindInfo.CallState = BindKeyState;

		//std::cout << "BindActor ptr: " << (void*)BindActor << "\n";
		BindInfo.BindFuncPtr = [BindActor, FuncPtr](FInputValue val)
			{
				//std::cout << "Lambda called! BindActor: " << (void*)BindActor << "\n";
				(BindActor->*FuncPtr)(val);
			};

		ActivatedIA[IAName]->BindFunc(BindInfo);
	}

	void ProcessInputTick();

private:
	bool ProcessKeyEvent(struct FKeyInputInfo KeyInputInfo);

private:
	FPInputMappingContext* IMC = nullptr;

	std::map<std::string, FPInputAction*> ActivatedIA;

	std::set<USHORT> PressedKeys;
};

