#include "ForestPearlEngine.h"

ForestPearlEngine& ForestPearlEngine::GetGameEngine()
{
    static ForestPearlEngine Singleton;

    return Singleton;
}

bool ForestPearlEngine::Initialize()
{
    //윈도우 생성
    Hwnd = CreateFPEWindow(WinClassName, WinName, WinWidth, WinHeight);

    if (Hwnd == nullptr)
    {
        //std::cout << "윈도우 생성 실패" << std::endl;
        system("pause");
        return false;
    }

    //윈도우 버퍼 생성
    FrontHdc = GetDC(Hwnd);
    BackHdc = CreateCompatibleDC(FrontHdc);
    BackBitmap = CreateCompatibleBitmap(FrontHdc, WinWidth, WinHeight);
    DefaultBitmap = (HBITMAP)SelectObject(BackHdc, BackBitmap);

    //RECT rcClient = {};
    //GetClientRect(m_hWnd, &rcClient);
    //m_width = rcClient.right - rcClient.left;
    //m_height = rcClient.bottom - rcClient.top;

#pragma region resource
    //m_pPlayerBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/redbird.png");
    //m_pEnemyBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/graybird.png");

#pragma endregion


    return true;
}

void ForestPearlEngine::GameLoop()
{
    while (true)
    {

    }
}

void ForestPearlEngine::Finalize()
{
}

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

    //std::cout << "width: " << rc.right - rc.left << " height: " << rc.bottom - rc.top << std::endl;

    HWND hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

    if (NULL == hWnd) return (HWND)(NULL);

    ::SetWindowText((HWND)hWnd, windowName);

    ShowWindow((HWND)hWnd, SW_SHOW);
    UpdateWindow((HWND)hWnd);

    return hWnd;
}

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
