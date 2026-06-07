#include "InputSystem.h"

#include <iostream>
#include "../Object/Components/InputMappingComponent.h"

FPInputSystem& FPInputSystem::GetInputSystem()
{
    static FPInputSystem Singleton;

    return Singleton;
}

void FPInputSystem::HandleRawInput(LPARAM LParam)
{
    //std::cout << "HandleRawInput Begin!\n";

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
        std::cout << "LParam Size is 0!\n";
        return;
    }

    // 2. 버퍼 할당 & 데이터 획득
    std::vector<BYTE> buffer(size);
    UINT result = GetRawInputData((HRAWINPUT)LParam, RID_INPUT, buffer.data(), &size, sizeof(RAWINPUTHEADER));
    if (result != size)
    {
        std::cout << "GetRawInputData is diffrent size!\n";
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
        HandleKeyboardInput(raw);
        break;
    default:
        std::cout << "지원하지 않는 입력!\n";
        break;
    }
}

void FPInputSystem::AddActivatedIMC(FPInputMappingComponent* IMC)
{
    if (IMC == nullptr)
    {
        std::cout << "IMC is null!\n";
        return;
    }

    auto it = std::find(ActivatedIMCs.begin(), ActivatedIMCs.end(), IMC);
    if (it != ActivatedIMCs.end()) {
        std::cout << "IMC is already exist!\n";
        return;
    }

    ActivatedIMCs.push_back(IMC);
}

void FPInputSystem::RemoveActivatedIMC(FPInputMappingComponent* IMC)
{
    if (IMC == nullptr)
    {
        std::cout << "IMC is null!\n";
        return;
    }

    auto it = std::find(ActivatedIMCs.begin(), ActivatedIMCs.end(), IMC);
    if (it == ActivatedIMCs.end()) {
        std::cout << "IMC is already not exist!\n";
        return;
    }

    ActivatedIMCs.erase(it, ActivatedIMCs.end());
}

void FPInputSystem::HandleMouseInput(RAWINPUT* RawInput)
{
    //std::cout << "HandleMouseInput Begin\n";
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

        if (KeyStates[VKey] == true && KeyStates[VKey] == bIsDown)
        {
            ChangedKeyState = EKeyState::Pressed;
        }
        else if (KeyStates[VKey] == true && KeyStates[VKey] != bIsDown)
        {
            ChangedKeyState = EKeyState::Down;
        }
        else if (KeyStates[VKey] == false && KeyStates[VKey] != bIsDown)
        {
            ChangedKeyState = EKeyState::Up;
        }

        for (FPInputMappingComponent* IMC : ActivatedIMCs)
        {
            IMC->OnKeyStateChanged(VKey, ChangedKeyState);
        }

        KeyStates[VKey] = bIsDown;
    }
}
