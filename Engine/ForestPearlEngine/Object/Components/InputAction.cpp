#include "InputAction.h"
#include <iostream>

void FPInputAction::OnInputAction(EKeyState KeyState, FPVector2 KeyValue)
{
	//std::cout << "FPInputAction::OnInputAction Begin\n";

	if (KeyState != CallKeyState)
		return;

	if (BindFuncPtr == nullptr)
		return;



	BindFuncPtr(KeyValue);
}