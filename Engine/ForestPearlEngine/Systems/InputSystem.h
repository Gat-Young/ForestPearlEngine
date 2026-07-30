#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <queue>
#include <set>
#include <utility>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")
#include "../InputValue.h"
#include "../FPGameInstanceSubSystem.h"

struct FKeyInputInfo
{
	USHORT VKey;
	enum EKeyState KeyState;
	struct FInputValue InputValue;
};

class FPInputMappingContext;

class FPInputSystem : public FPGameInstanceSubSystem
{
private:
	

public:
	FPInputSystem() = default;

	void HandleRawInput(LPARAM LParam);

	void TickInputSystem();

	bool bIsKeyDown(USHORT VKey);

	std::queue<FKeyInputInfo>& GetInputQueue() { return InputQueue; }

	std::set<USHORT>& GetCheckPressedKeys() { return CheckPressedKeys; }

	void ResetKeyStates();

private:
	void HandleMouseInput(RAWINPUT* RawInput);
	void HandleKeyboardInput(RAWINPUT* RawInput);
	void HandleGamepadInput();

private:
	int  MouseX = 0, MouseY = 0;
	bool KeyStates[256] = {false};
	std::queue<FKeyInputInfo> InputQueue;
	std::set<USHORT> CheckPressedKeys;
	XINPUT_STATE PrevGamepadState = {};
};

