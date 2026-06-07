#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>

class FPInputMappingComponent;

class FPInputSystem
{
private:
	FPInputSystem() = default;

public:
	static FPInputSystem& GetInputSystem();

	void HandleRawInput(LPARAM LParam);

	void AddActivatedIMC(FPInputMappingComponent* IMC);
	void RemoveActivatedIMC(FPInputMappingComponent* IMC);

private:
	void HandleMouseInput(RAWINPUT* RawInput);
	void HandleKeyboardInput(RAWINPUT* RawInput);

private:
	int  MouseX = 0, MouseY = 0;
	bool KeyStates[256] = {false};
	std::vector<FPInputMappingComponent*> ActivatedIMCs;
};

