#pragma once
#pragma warning(disable:4996)

#include "Mia.h"

////////////////////////////////////////////////////////////
//
// 전역변수 선언 영역
//
typedef IMiaDevice9* LPDEVICE;

extern BOOL g_ShowFrame;
extern HWND g_hWnd;
extern MIADISPLAYMODE g_Mode;  // 렌더 타겟 해상도 정보
extern LPMIADEVICE9 g_pDevice;

////////////////////////////////
// 장치 생성하기 및 해제

int MiaSetUp(HWND hwnd);
void MiaRelease();

// 타이머의 초당 프레임률(fps)을 출력
void PutFPS(int x, int y);

float GetEngineTime();

// 텍스트 출력
void DrawText(int x, int y, COLORREF color, TCHAR* msg, ...);

