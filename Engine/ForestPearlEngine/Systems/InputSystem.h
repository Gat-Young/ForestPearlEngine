#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <queue>
#include <utility>

class FPInputMappingContext;

class FPInputSystem
{
private:
	FPInputSystem() = default;

public:
	static FPInputSystem& GetInputSystem();

	void HandleRawInput(LPARAM LParam);

	bool bIsKeyDown(USHORT VKey);

	std::queue<std::pair<USHORT, enum EKeyState>>& GetInputQueue() { return InputQueue; }

private:
	void HandleMouseInput(RAWINPUT* RawInput);
	void HandleKeyboardInput(RAWINPUT* RawInput);

private:
	int  MouseX = 0, MouseY = 0;
	bool KeyStates[256] = {false};
	std::queue<std::pair<USHORT, enum EKeyState>> InputQueue;
};

