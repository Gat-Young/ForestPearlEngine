#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <queue>
#include <utility>
#include "../InputValue.h"

struct FKeyInputInfo
{
	USHORT VKey;
	enum EKeyState KeyState;
	struct FInputValue InputValue;
};

class FPInputMappingContext;

class FPInputSystem
{
private:
	FPInputSystem() = default;

public:
	static FPInputSystem& GetInputSystem();

	void HandleRawInput(LPARAM LParam);

	bool bIsKeyDown(USHORT VKey);

	std::queue<FKeyInputInfo>& GetInputQueue() { return InputQueue; }

private:
	void HandleMouseInput(RAWINPUT* RawInput);
	void HandleKeyboardInput(RAWINPUT* RawInput);

private:
	int  MouseX = 0, MouseY = 0;
	bool KeyStates[256] = {false};
	std::queue<FKeyInputInfo> InputQueue;
};

