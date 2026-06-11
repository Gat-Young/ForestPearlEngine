#pragma once
#include <vector>
#include <functional>
#include "InputMappingContext.h"
#include "../../Systems/KeyStateEnum.h"
#include "../../Define/FPMath.h"
#include <iostream>

class FPInputAction;

class FPInputComponent
{
public:
	FPInputComponent();
	~FPInputComponent();

	FPInputAction& GetIA() { return *IA; }
	FPInputMappingContext& GetIMC() { return *IMC; }

	void AddMappingKey(USHORT VKey, FMappingInfo MappingInfo);
	//void RemoveMappingKey(USHORT VKey);

	template<typename TObj>
	void BindMethod(TObj* BindActor, EKeyState BindKeyState, void(TObj::* FuncPtr)(FPVector2))
	{
		//std::cout << "BindActor ptr: " << (void*)BindActor << "\n";
		BindFuncPtr = [BindActor, FuncPtr](FPVector2 val)
			{
				//std::cout << "Lambda called! BindActor: " << (void*)BindActor << "\n";
				(BindActor->*FuncPtr)(val);
			};
		CallKeyState = BindKeyState;
	}

	void ProcessInputTick();

private:
	void ProcessKeyEvent(USHORT VKey, EKeyState KeyState);

private:
	FPInputAction* IA = nullptr;
	FPInputMappingContext* IMC = nullptr;
	EKeyState CallKeyState;
	std::function<void(FPVector2)> BindFuncPtr = nullptr;
};

