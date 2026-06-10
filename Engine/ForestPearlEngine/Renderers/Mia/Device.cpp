#include "windows.h"
#include "tchar.h"
#include "mmsystem.h"
#include "stdio.h"

#include "Device.h"

////////////////////////////////////////////////////////////
//
// 데이터 정의 영역
//
///////////////////////////////////
// 화면 모드 설정 전역 변수
LPMIA  g_pMia = NULL;
LPMIADEVICE9 g_pDevice = NULL;

///////////////////////////////////
// 화면 모드 설정 전역 변수
MIADISPLAYMODE g_Mode = { 800, 600 };  //4:3

///////////////////////////////////
// 렌더타겟(Back-Buffer) 구성용 핸들
//백버퍼 접근
#define  hRT  g_pDevice->GetRT()

///////////////////////////////////
// 시스템 폰트
HFONT g_hSysFont = NULL;
COLORREF g_SysFnColor = RGB(0, 255, 0);

///////////////////////////////////
// 기타 상태 변수
BOOL g_ShowFrame = FALSE;





////////////////////////////////////////////////////////////
//
// 함수 정의 영역
//

//////////////////////////////////////////////
//
// 장치 생성하기 및 해제
//
int MiaSetUp(HWND hwnd)
{
	// 백버퍼용 렌더타겟을 생성
	g_pMia = MiaCreate9(MIA_VERSION);
	if (MC_INVALIED(g_pMia))
	{
		return MC_FAIL;
	}

	MIAPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.Width = g_Mode.Width;
	pp.Height = g_Mode.Height;
	pp.BackBuffercnt = 1;
	pp.Windowed = TRUE;

	g_pMia->CreateDevice(g_hWnd, &pp, MIACREATE_SOFTWARE_VERTEXPROCESSING, &g_pDevice);
	if (MC_INVALIED(g_pDevice))
	{
		return MC_FAIL;
	}

	// 시스템 폰트 생성
	g_hSysFont = CreateFont(
		12, 6,
		0, 0, 1, 0, 0, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE,
		_T("굴림")
	);

	if (g_hSysFont == NULL)
	{
		return MC_FAIL;
	}

	SelectObject(hRT, g_hSysFont);
	return MC_OK;
}

void MiaRelease()
{
	//ReleaseRenderTarget();
	DeleteObject(g_hSysFont);

	// 렌더링 객체 제거.
	SafeRelease(g_pDevice);		//디바이스 제거
	SafeRelease(g_pMia);		//미아 제거
}

//////////////////////////////////////////////
//
// 게임 장면 렌더링
//
///////////////////////////////////
// 타이머의 초당 프레임률(fps)을 출력
// params : 출력할 화면 좌표 (2d x,y)
void PutFPS(int x, int y)
{
	static UINT frm = 0;
	static float fps = 0.0f;
	++frm;
	static ULONGLONG oldtime = GetTickCount64();
	ULONGLONG nowtime = GetTickCount64();

	UINT time = (UINT)(nowtime - oldtime);
	if (time >= 1000)
	{
		fps = (float)(frm * 1000) / (float)time;
		frm = 0;
		oldtime = nowtime;
	}

	DrawText(x, y, RGB(255, 255, 255), _T("FPS=%.1f/%d      "), fps, time);
}

float GetEngineTime()
{
	static ULONGLONG oldtime = GetTickCount64();
	ULONGLONG nowtime = GetTickCount64();
	float dTime = (nowtime - oldtime) * 0.001f;
	oldtime = nowtime;

	return dTime;
}

///////////////////////////////////
// 텍스트 출력
void DrawText(int x, int y, COLORREF color, TCHAR* msg, ...)
{
	TCHAR buff[2048] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);

	RECT rc = { x, y, x + g_Mode.Width, y + g_Mode.Height };

	SetTextColor(hRT, color);
	DrawText(hRT, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(hRT, RGB(255, 255, 255));
}