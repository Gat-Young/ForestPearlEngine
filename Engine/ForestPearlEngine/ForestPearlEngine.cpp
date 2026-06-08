#include "ForestPearlEngine.h"
#include "../ForestPearlEngine/Object/Object.h"
#include "../ForestPearlEngine/Object/Actor.h"
#include "Renderers/Renderer.h"
#include "GameProjectLoader.h"
#include <iostream>

//싱글톤 엔진 객체 가져오기
ForestPearlEngine& ForestPearlEngine::GetGameEngine()
{
    static ForestPearlEngine Singleton;

    return Singleton;
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

    //Render 등록
    Render = new Renderer();
    Render->InitializeRenderer(32, Hwnd);

    LoadClassRegist();

    GameInstance = std::make_unique<FPGameInstance>();
    return true;
}

//BaseWorld 생성 및 Begin Play 수행
bool ForestPearlEngine::Initialize()
{
    FPGameInstance::Get().OpenLevel(ReturnStartWorld());
    return true;
}

//메인 게임 루프
void ForestPearlEngine::GameLoop()
{
    GameObjectList[0]->BeginPlay();

    Render->MakeVB(GameActorRenderList);

    for (int i=1; i<GameObjectList.size();++i)
    {
        FPObject* obj = GameObjectList[i];
        obj->BeginPlay();
    }

    while (bEngineLoop)
    {
        if (!MessagePump())
        {
            break;
        }

        for (auto& obj : GameObjectList)
        {
            obj->Tick();
        }

        //Rendering
        Render->ObjectRendering(GameActorRenderList);
        Render->UIRendering(GameUIRenderList);
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
    for (auto& obj : GameObjectList)
    {
        delete(obj);
    }
}

void ForestPearlEngine::AddObjectTable(FPObject* obj)
{
    GameObjectList.push_back(obj);
}

void ForestPearlEngine::AddRenderTable(FPActor* actor)
{
    GameActorRenderList.push_back(actor);
}

void ForestPearlEngine::AddUITable(FPActor* actor)
{
    GameUIRenderList.push_back(actor);
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
    case WM_KEYDOWN:
    {

    }break;

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