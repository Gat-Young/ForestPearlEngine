#include "ForestPearlEngine.h"
#include "../ForestPearlEngine/Object/Object.h"
#include "../ForestPearlEngine/Object/Actor.h"
#include "Renderers/RenderingDevice.h"
#include "Renderers/Renderer.h"
#include "GameProjectLoader.h"
#include "AssetManager.h"
#include "MeshRenderList.h"
#include "TextRenderList.h"
#include "CameraList.h"
#include "Systems/InputSystem.h"
//#include <iostream>
#include "MCLOG.h"

//싱글톤 엔진 객체 가져오기
ForestPearlEngine& ForestPearlEngine::GetGameEngine()
{
    static ForestPearlEngine GameEngineSingleton;

    return GameEngineSingleton;
}

//엔진 부팅 및 기본 설정 모듈 불러오기
bool ForestPearlEngine::PreInitialize()
{
    //윈도우 생성
    Hwnd = CreateFPEWindow(WinClassName, WinName, WinWidth, WinHeight);

    if (Hwnd == nullptr)
    {
        //std::cout << "윈도우 생성 실패" << std::endl;
        system("pause");
        return false;
    }

    // InputSystem 만들기
    RegisterFPRawInputDevices();
    FPInputSystem::GetInputSystem();

    // RenderDevice 생성
    RenderDevice = &RenderingDevice::GetRenderingDevice();

    //Render 등록
    Render = new Renderer(*RenderDevice);
    Render->InitializeRenderer(Hwnd);
    
    GameProjectClassRegistry::Get();

    LoadClassRegist();

    //AssetManager 생성
    AssetManager::Get();
    LoadAssets();

    MeshRenderList::Get();
    TextRenderList::Get();
    CameraList::Get();
    FPGameInstance::Get();

    return true;
}

//BaseWorld 생성 및 Begin Play 수행
bool ForestPearlEngine::Initialize()
{
    FPGameInstance::Get().OpenLevel(ReturnStartWorld());
    FPGameInstance::Get().Initialize();
    FPGameInstance::Get().BeginPlay();
    return true;
}

//메인 게임 루프
void ForestPearlEngine::GameLoop()
{
    while (bEngineLoop)
    {
        if (!MessagePump())
        {
            break;
        }

        FPInputSystem::GetInputSystem().TickInputSystem();

        FPGameInstance::Get().Tick();

        //Rendering
        Render->ClearBackBuffer();
        Render->ObjectRendering();
        Render->UIRendering();
        Render->RenderTargetPresent();
    }

}

//엔진 루프를 종료
void ForestPearlEngine::StopEngine()
{
    bEngineLoop = false;
}

//엔진 종료 및 메모리 해제
void ForestPearlEngine::Finalize()
{
    FPGameInstance::Get().Finalize();
    Render->Finalize();
}

//윈도우 생성 함수
HWND ForestPearlEngine::CreateFPEWindow(const wchar_t* className, const wchar_t* windowName, const int width, const int height)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszClassName = className;
    wc.lpfnWndProc = WndProc; // 윈도우 프로시저(함수)의 포인터 등록


    ATOM classId = 0;
    if (!GetClassInfoEx(HINSTANCE(), className, &wc))
    {
        classId = RegisterClassEx(&wc);

        if (0 == classId) return NULL;
    }

    RECT rc = { 0, 0, width, height };

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

    HWND hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

    if (NULL == hWnd) return (HWND)(NULL);

    ::SetWindowText((HWND)hWnd, windowName);

    ShowWindow((HWND)hWnd, SW_SHOW);
    UpdateWindow((HWND)hWnd);

    return hWnd;
}

//윈도우 콜백 함수
LRESULT CALLBACK ForestPearlEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INPUT:
        //MCLOG(LogMC, "");
        FPInputSystem::GetInputSystem().HandleRawInput(lParam);
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;

    case WM_ACTIVATE:
        FPInputSystem::GetInputSystem().ResetKeyStates();
        break;

    case WM_QUIT:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//윈도우 메시지 펌프
int ForestPearlEngine::MessagePump()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg)); //msg 영역 초기화

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return FALSE;

            //나머지 메시지 리턴
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            return TRUE;
        }
    }

    return FALSE;
}

void ForestPearlEngine::RegisterFPRawInputDevices()
{
        // Raw Input 등록
    RAWINPUTDEVICE rid[2] = {};
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x02;  // 마우스
    rid[0].dwFlags = 0;
    rid[0].hwndTarget = Hwnd;

    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = 0x06;  // 키보드
    rid[1].dwFlags = 0;
    rid[1].hwndTarget = Hwnd;

    RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE));
}



//렌더러 정보 반환 함수
const TCHAR* ForestPearlEngine::GetAdapterDescription(int index) { return Render->GetRenderingDevice().GetAdapterDescription(index); }
UINT ForestPearlEngine::GetAdapterVendorID(int index) { return Render->GetRenderingDevice().GetAdapterVendorID(index); }
UINT ForestPearlEngine::GetAdapterDeviceID(int index) { return Render->GetRenderingDevice().GetAdapterDeviceID(index); }
UINT ForestPearlEngine::GetAdapterSubSysID(int index) { return Render->GetRenderingDevice().GetAdapterSubSysID(index); }
UINT ForestPearlEngine::GetAdapterRevision(int index) { return Render->GetRenderingDevice().GetAdapterRevision(index); }
SIZE_T ForestPearlEngine::GetAdapterVideoMem(int index) { return Render->GetRenderingDevice().GetAdapterVideoMem(index); }
SIZE_T ForestPearlEngine::GetAdapterSystemMem(int index) { return Render->GetRenderingDevice().GetAdapterSystemMem(index); }
SIZE_T ForestPearlEngine::GetAdapterSharedSysMem(int index) { return Render->GetRenderingDevice().GetAdapterSharedSysMem(index); }
LONG ForestPearlEngine::GetAdapterLuidHighPart(int index) { return Render->GetRenderingDevice().GetAdapterLuidHighPart(index); }
DWORD ForestPearlEngine::GetAdapterLuidLowPart(int index) { return Render->GetRenderingDevice().GetAdapterLuidLowPart(index); }

//모니터 정보 반환
const TCHAR* ForestPearlEngine::GetMonitorName(int AdapterIndex, int MonitorIndex) { return Render->GetRenderingDevice().GetMonitorName(AdapterIndex, MonitorIndex); };
RECT ForestPearlEngine::GetDesktopCoordinates(int AdapterIndex, int MonitorIndex) { return Render->GetRenderingDevice().GetDesktopCoordinates(AdapterIndex, MonitorIndex); };

//VRAM 정보 반환
double ForestPearlEngine::GetVRAMBudget(int AdapterIndex) { return Render->GetRenderingDevice().GetVRAMBudget(AdapterIndex); }
double ForestPearlEngine::GetVRAMCurrUsage(int AdapterIndex) { return Render->GetRenderingDevice().GetVRAMCurrUsage(AdapterIndex); }
double ForestPearlEngine::GetVRAMAvailableForReservation(int AdapterIndex) { return Render->GetRenderingDevice().GetVRAMAvailableForReservation(AdapterIndex); }
double ForestPearlEngine::GetVRAMCurrReservation(int AdapterIndex) { return Render->GetRenderingDevice().GetVRAMCurrReservation(AdapterIndex); }

//장치 개수 반환
int ForestPearlEngine::GetAdapterSize() { return Render->GetRenderingDevice().GetAdapterSize(); };
//장치의 모니터 개수 반환
int ForestPearlEngine::GetAdapterMonitorSize(int index) { return Render->GetRenderingDevice().GetAdapterMonitorSize(index); };

const TCHAR* ForestPearlEngine::GetSrtFeatureLevel() { return Render->GetRenderingDevice().GetSrtFeatureLevel(); };
UINT ForestPearlEngine::GetWidth() { return Render->GetRenderingDevice().GetWidth(); };
UINT ForestPearlEngine::GetHeight() { return Render->GetRenderingDevice().GetHeight(); };