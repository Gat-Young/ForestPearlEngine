#include "GDI.h"
#include <iostream>


SimpleGDI::SimpleGDI(HWND hwnd) : Hwnd(hwnd)
{
    //윈도우 사이즈 가져오기
    RECT rect;
    GetClientRect(hwnd, &rect);
    WinWidth = rect.right - rect.left;
    WinHeight = rect.bottom - rect.top;

    //윈도우 버퍼 생성
    FrontHdc = GetDC(Hwnd);
    BackHdc = CreateCompatibleDC(FrontHdc);
    BackBitmap = CreateCompatibleBitmap(FrontHdc, WinWidth, WinHeight);
    DefaultBitmap = (HBITMAP)SelectObject(BackHdc, BackBitmap);
}

void SimpleGDI::Rendering(std::vector<FPActor*> RenderList)
{
    //Clear the back buffer
    ::PatBlt(BackHdc, 0, 0, WinWidth, WinHeight, WHITENESS);

    for (FPActor* Actor : RenderList)
    {
        DrawCollider(BackHdc, Actor);
    }

    //메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
    BitBlt(FrontHdc, 0, 0, WinWidth, WinHeight, BackHdc, 0, 0, SRCCOPY);
}

void SimpleGDI::DrawCollider(HDC hdc, FPActor* Actor)
{
    //std::cout << "Draw Collide " << "\n";
    COLORREF Color = RGB(255, 0, 0);

    HPEN hPen = CreatePen(PS_SOLID, 2, Color);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Ellipse(hdc,Actor->Transform.x - 30,
                Actor->Transform.y - 30,
                Actor->Transform.x + 30,
                Actor->Transform.y + 30);


    // 이전 객체 복원 및 펜 삭제
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}