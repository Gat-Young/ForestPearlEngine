#pragma once

////////////////////////////////
// 시스템 / 플랫폼 헤더
#include "windows.h"
#include "tchar.h"
#include "math.h"
#include "stdio.h"
#include "vector"
#include "algorithm"
#include "string"

////////////////////////////////
// Mia SW Renderer 헤더
// Yena SWR 장치/공통 인터페이스
#include "mcDefine.h"                       // 기본/공통 상수 정의
#include "mcError.h"                        // 에러처리 헤더
#include "mcGUID.h"                        // Mia SWR COM / GUID 헤더
#include "mcUnknownbase.h"           // COM 헤더, 기반 인터페이스 헤더
#include "mcx9types.h"                    // 자료형 정의 : D3D9 대응
#include "mcx9.h"                             // 장치 헤더 : D3D9 대응


/* --> 이하 클래스, 인터페이스로 전환, mcx9.h 로 이동


/////////////////////////////////////////////////////////////
//
// 렌더타겟 (백버퍼) - 정보 설정 구조체
//
//struct MIAPRESENT_PARAMETERS
//{
//	DWORD Width;
//	DWORD Height;
//	DWORD BackBuffercnt;
//	BOOL  Windowed;
//};

////////////////////////////////
// 렌더 타겟 해상도 정보 구조체
//struct MIADISPLAYMODE
//{
//	DWORD Width, Height;
//};

/////////////////////////////////////////////////////////////
//
// 정점 처리 - 가속화 정의
//
//enum BEHAVIOR_FLAG
//{
//	MIACREATE_SOFTWARE_VERTEXPROCESSING,
//	MIACREATE_HARDWARE_VERTEXPROCESSING
//};

/////////////////////////////////////////////////////////////
//
// 클래스 전방 선언
//
class B3MiaDevice9;
typedef B3MiaDevice9 B3MIADEVICE9;
typedef B3MiaDevice9* LPB3MIADEVICE9;

#define MIA_VERSION 9

class B3Mia
{
	friend B3Mia* B3MiaCreate9(DWORD ver);

protected:
	B3Mia(void);

public:
	virtual ~B3Mia(void);

/////////////////////////////////////////////////////////////
//
// 외부 노출 메소드
//
public:
	//int CreateDevice(HWND hwnd, MIAPRESENT_PARAMETERS* pp, DWORD vp, LPB3MIADEVICE9* pDev);
};

typedef B3Mia* LPB3MIA;

// Mia 개체 생성 함수 (D3D9 대응)
//B3Mia* B3MiaCreate9(DWORD ver);

class B3MiaDevice9
{
	friend class B3Mia;

protected:
	HWND	m_hWnd;
	MIAPRESENT_PARAMETERS m_PresentParam;
	DWORD m_VertexProcessing;

protected:
	HBITMAP  m_hBmpRT;
	HDC  m_hSurfaceRT;
	COLORREF  m_BkColor;

protected:
	B3MiaDevice9(void);
public:
	virtual ~B3MiaDevice9(void);

protected:
	int _RenderTargetCreate();
	void _RenderTargetRelease();

public:
	HDC	 GetRT();
	COLORREF  GetBkColor();

public:
	int  BeginScene();
	int  EndScene();
	int  Clear(COLORREF color);
	int  Present();
};
*/
