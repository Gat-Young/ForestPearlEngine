#include "InputAction.h"
#include <iostream>

void FPInputAction::OnInputAction(EKeyState KeyState, FInputValue KeyValue)
{
	std::cout << "FPInputAction::OnInputAction Begin\n";
}
