#include "InputSystem.h"
#include "../MCLOG.h"
#include "../Object/Components/InputMappingContext.h"

void FPInputSystem::HandleRawInput(LPARAM LParam)
{
    //MCLOG(LogMC,"");

    // 1. 버퍼 크기 조회
    UINT size = 0;
    GetRawInputData(
        (HRAWINPUT)LParam,
        RID_INPUT,
        nullptr,
        &size,
        sizeof(RAWINPUTHEADER)
    );
    if (size == 0)
    {
        MCLOG(ErrorMC, "LParam Size is 0!");
        return;
    }

    // 2. 버퍼 할당 & 데이터 획득
    std::vector<BYTE> buffer(size);
    UINT result = GetRawInputData((HRAWINPUT)LParam, RID_INPUT, buffer.data(), &size, sizeof(RAWINPUTHEADER));
    if (result != size)
    {
        MCLOG(ErrorMC, "GetRawInputData is diffrent size!");
        return; // 크기 불일치 — 오류
    }

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.data());

    switch (raw->header.dwType)
    {
        //마우스
    case RIM_TYPEMOUSE:
        HandleMouseInput(raw);
        break;
        //키보드
    case RIM_TYPEKEYBOARD:
        //MCLOG(LogMC, "RIM_TYPEKEYBOARD Begin");
        HandleKeyboardInput(raw);
        break;
    default:
        MCLOG(ErrorMC, "지원하지 않는 입력!");
        break;
    }
}

void FPInputSystem::TickInputSystem()
{
    //HWND hwnd = GetActiveWindow();
    //bool bIsActive = (hwnd == GetForegroundWindow());
    //if (!bIsActive)
    //{
    //    for (bool& AllKeys : KeyStates)
    //    {
    //        AllKeys = false;
    //    }
    //    return;
    //}

    for (USHORT Key : CheckPressedKeys)
    {
        if (bIsKeyDown(Key))
        {
            FInputValue InputValue = { 1.0f, 0.0f, 0.0f, true, 1.0f };
            FKeyInputInfo PressedKeyEvent = { Key, EKeyState::Pressed, InputValue };
            InputQueue.push(PressedKeyEvent);
        }
    }

    HandleGamepadInput();
}

bool FPInputSystem::bIsKeyDown(USHORT VKey)
{
    if (VKey < 256)
    {
        return KeyStates[VKey];
    }
    else
    {
        return false;
    }
}

void FPInputSystem::ResetKeyStates()
{
    for (bool& AllKeys : KeyStates)
    {
        AllKeys = false;
    }
}

void FPInputSystem::HandleMouseInput(RAWINPUT* RawInput)
{
    //MCLOG(LogMC, "");

    RAWMOUSE& m = RawInput->data.mouse;

    bool isAbsolute = (m.usFlags & MOUSE_MOVE_ABSOLUTE) != 0;
    static int  rawDeltaX = 0, rawDeltaY = 0;
    static int  mouseX = 0, mouseY = 0;

    USHORT VKey;
    EKeyState ChangedKeyState = EKeyState::None;

    if (m.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
    {
        VKey = VK_LBUTTON;
        ChangedKeyState = EKeyState::Down;
    }
    else if (m.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
    {
        VKey = VK_LBUTTON;
        ChangedKeyState = EKeyState::Up;
    }
    else if (m.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
    {
        VKey = VK_RBUTTON;
        ChangedKeyState = EKeyState::Down;
    }
    else if (m.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
    {
        VKey = VK_RBUTTON;
        ChangedKeyState = EKeyState::Up;
    }
    else if (m.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
    {
        VKey = VK_MBUTTON;
        ChangedKeyState = EKeyState::Down;
    }
    else
    {
        return;
    }

    POINT pt;
    HWND hwnd = GetActiveWindow();
    GetCursorPos(&pt);
    ScreenToClient(hwnd, &pt);

    float ValueX = (pt.x -400) / 400.0f;
    float ValueY = (pt.y - 300) / 300.0f;
    ValueY *= -1;

    FInputValue InputValue = { ValueX, ValueY, 0.0f, true, 0.0f };

    FKeyInputInfo KeyInputInfo = { VKey, ChangedKeyState, InputValue };
    InputQueue.push(KeyInputInfo);
}

void FPInputSystem::HandleKeyboardInput(RAWINPUT* RawInput)
{
    //std::cout << "HandleKeyboardInput Begin\n";

    RAWKEYBOARD& KeyBoard = RawInput->data.keyboard;
    USHORT VKey = KeyBoard.VKey;
    USHORT Flags = KeyBoard.Flags;

    bool bIsDown = !(Flags & RI_KEY_BREAK);  // BREAK = keyup

    if (VKey < 256)
    {
        EKeyState ChangedKeyState = EKeyState::None;

        if (KeyStates[VKey] == true && bIsDown == true)
        {
            //ChangedKeyState = EKeyState::Pressed;
            return;
        }
        else if (KeyStates[VKey] == false && bIsDown == true)
        {
            ChangedKeyState = EKeyState::Down;
        }
        else if (KeyStates[VKey] == true && bIsDown == false)
        {
            ChangedKeyState = EKeyState::Up;
        }

        FInputValue InputValue = { 1.0f, 0.0f, 0.0f, true, 1.0f };
        FKeyInputInfo KeyInputInfo = { VKey, ChangedKeyState, InputValue };

        InputQueue.push(KeyInputInfo);
        KeyStates[VKey] = bIsDown;
    }
}

void FPInputSystem::HandleGamepadInput()
{
    XINPUT_STATE state = {};
    if (XInputGetState(0, &state) != ERROR_SUCCESS)
        return;

    // 버튼 처리 (8개 버튼만 우선 예시: A, B, X, Y, LB, RB, Start, Back)
    struct { WORD Mask; USHORT VKey; } Buttons[] = {
        { XINPUT_GAMEPAD_A, 102 },
        { XINPUT_GAMEPAD_B, 103 },
        { XINPUT_GAMEPAD_X, 104 },
        { XINPUT_GAMEPAD_Y, 105 },
        { XINPUT_GAMEPAD_LEFT_SHOULDER, 106 },
        { XINPUT_GAMEPAD_RIGHT_SHOULDER, 107 },
        { XINPUT_GAMEPAD_START, 108 },
        { XINPUT_GAMEPAD_BACK, 109 },
    };

    for (auto& btn : Buttons)
    {
        bool bIsDown = (state.Gamepad.wButtons & btn.Mask) != 0;
        bool bWasDown = (PrevGamepadState.Gamepad.wButtons & btn.Mask) != 0;

        EKeyState ChangedKeyState = EKeyState::None;
        if (!bWasDown && bIsDown)
            ChangedKeyState = EKeyState::Down;
        else if (bWasDown && !bIsDown)
            ChangedKeyState = EKeyState::Up;
        else if (bWasDown && bIsDown)
            ChangedKeyState = EKeyState::Pressed;
        else
            continue;

        FInputValue InputValue = { 1.0f, 0.0f, 0.0f, true, 1.0f };
        FKeyInputInfo KeyInputInfo = { btn.VKey, ChangedKeyState, InputValue };
        //MCLOG(LogMC, "VKey : %d", btn.VKey);
        InputQueue.push(KeyInputInfo);
    }

    // 왼쪽 스틱 처리
    float LX = state.Gamepad.sThumbLX / 32767.0f;
    float LY = state.Gamepad.sThumbLY / 32767.0f;

    if (fabsf(LX) > 0.1f || fabsf(LY) > 0.1f) // 데드존
    {
        FInputValue InputValue = { LX, LY, 0.0f, true, 1.0f };
        FKeyInputInfo KeyInputInfo = { 100, EKeyState::Pressed, InputValue }; // 스틱용 VKey 임시값
        //MCLOG(LogMC, "");
        InputQueue.push(KeyInputInfo);
    }

    // 오른쪽 스틱 처리
    float RX = state.Gamepad.sThumbRX / 32767.0f;
    float RY = state.Gamepad.sThumbRY / 32767.0f;

    if (fabsf(RX) > 0.1f || fabsf(RY) > 0.1f) // 데드존
    {
        FInputValue InputValue = { RX, RY, 0.0f, true, 1.0f };
        FKeyInputInfo KeyInputInfo = { 101, EKeyState::Pressed, InputValue }; // 스틱용 VKey 임시값
        //MCLOG(LogMC, "R Stick Trigger");
        InputQueue.push(KeyInputInfo);
    }

    PrevGamepadState = state;
}
