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

enum class XBOX_GAMEPAD
{
	GAMEPAD_LSTICK = 0x100,
	GAMEPAD_RSTICK = 0x101,

	GAMEPAD_A = 0x0102,
	GAMEPAD_B = 0x0103,
	GAMEPAD_X = 0x0104,
	GAMEPAD_Y = 0x0105,

	GAMEPAD_LEFT_SHOULDER = 0x0106,
	GAMEPAD_RIGHT_SHOULDER = 0x0107,

	GAMEPAD_START = 0x0108,
	GAMEPAD_BACK = 0x0109,
	GAMEPAD_DPAD_UP = 0x010A,
	GAMEPAD_DPAD_DOWN = 0x010B,
	GAMEPAD_DPAD_LEFT = 0x010C,
	GAMEPAD_DPAD_RIGHT = 0x010D,

	GAMEPAD_LEFT_THUMB = 0x010E,
	GAMEPAD_RIGHT_THUMB = 0x010F,

	GAMEPAD_LEFT_TRIGER = 0x0110,
	GAMEPAD_RIGHT_TRIGER = 0x0111
};
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

