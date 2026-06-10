#pragma warning(disable:4996)
#include "Windows.h"
#include "stdio.h"
#include "tchar.h"

#include "Device.h"
#include "Render.h"

//#include <cmath>

////////////////////////////////////////////////////////////
//
// 데이터 정의 영역
//

TCHAR* g_WindowName = _T("Mia::SW Renderer 03 : Device (ver.Mia)(Interface)");

// 전역 데이터
// 정점 버퍼 : 정점 데이터 관리용 개체
LPMIAVERTEXBUFFER9 g_pVB = NULL;

// 정점 구조체
struct COLVTX
{
	float x, y;		// 정점 좌표
	DWORD diff;     // 정점 색
};

// 정점 포멧의 플래그 조합
#define FVF_COLVTX (B3MFVF_XY | B3MFVF_DIFFUSE)

// 렌더링 상태 변수
BOOL g_bWireFrame = FALSE;  // 와이어 프레임 출력 플래그
BOOL g_bCulling = FALSE;    // 뒷면제거 플래그

////////////////////////////////////////////////////////////
//
// 함수 정의 영역
//
///////////////////////////////////
// 게임 데이터 및 렌더링 자원을 불러옴
// 성공하면 TRUE, 실패하면 FALSE
int DataLoading()
{
	ObjLoad();

	return TRUE;
}

///////////////////////////////////
// 게임 데이터 및 렌더링 자원을 해제
void DataRelease()
{
	ObjRelease();
}

int ObjLoad()
{
	COLVTX	Vertices[] = {
		// 정삼각형
		{  50.0f, 250.0f, 0xffff0000 },
		{ 150.0f,  50.0f, 0xff00ff00 },
		{ 250.0f, 250.0f, 0xff00ffff },

		// 역삼각형
		{  50.0f, 250.0f, 0xffff0000 },
		{ 150.0f, 450.0f, 0xff00ff00 },
		{ 250.0f, 250.0f, 0xff00ffff },

		// CW
		{  300.0f, 500.0f, 0xffff0000 },
		{  400.0f, 300.0f, 0xff00ff00 },
		{  480.0f, 430.0f, 0xff00ffff },

		// CCW
		{  500.0f, 430.0f, 0xffff0000 },
		{  680.0f, 500.0f, 0xff00ff00 },
		{  600.0f, 300.0f, 0xff00ffff },

		// 직각 삼각형
		{  10.0f,  30.0f, 0xffff0000 },
		{  10.0f, 100.0f, 0xff00ff00 },
		{  60.0f, 100.0f, 0xff00ffff },

		// 직각 삼각형2
		{  40.0f,  30.0f, 0xffff0000 },
		{  90.0f,  30.0f, 0xff00ff00 },
		{  90.0f, 100.0f, 0xff00ffff },
	};

	if (MC_FAILED(g_pDevice->CreateVertexBuffer(sizeof(Vertices), 0, FVF_COLVTX, B3MPOOL_SYSTEMMEM, &g_pVB, NULL)))
	{
		return MC_FALSE;
	}

	VOID* pBuff;
	if (MC_FAILED(g_pVB->Lock(0, sizeof(Vertices), (void**)&pBuff, 0)))
	{
		return MC_FALSE;
	}

	memcpy(pBuff, Vertices, sizeof(Vertices));
	g_pVB->Unlock();

	return MC_OK;
}

void ObjRelease()
{
	SafeRelease(g_pVB);
}

//void ObjUpdate(float dTime)
//{
//}


//////////////////////////////////////////////
//
// 오브젝트 렌더링
//
void ObjDraw()
{
	g_pDevice->SetStreamSource(0, g_pVB, 0, sizeof(COLVTX));

	g_pDevice->SetFVF(FVF_COLVTX);

	g_pDevice->DrawPrimitive(B3MPT_TRIANGLELIST, 0, 6);

}

//////////////////////////////////////////////
//
// 엔진 및 시스템 상태 갱신
//
void SystemUpdate()
{
	if (IsKeyUp(VK_SPACE)) g_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F5))		g_bCulling ^= TRUE;

	g_pDevice->SetRenderState(B3MRS_FILLMODE, g_bWireFrame ? B3MFILL_WIREFRAME : B3MFILL_SOLID);
	g_pDevice->SetRenderState(B3MRS_CULLMODE, g_bCulling ? B3MCULL_CCW : B3MCULL_NONE);
}

//////////////////////////////////////////////
//
// 게임 장면 렌더링
//
void SceneRender()
{
	// 정점 정보 보기 끄고 키기
	//if (IsKeyUp(VK_F2)) g_bShowVtxInfo ^= TRUE;

	// 엔진 상태 갱신
	SystemUpdate();

	g_pDevice->BeginScene();

	g_pDevice->Clear(RGB(80, 80, 80));

	ObjDraw();

	PutFPS(1, 1);
	ShowInfo();

	g_pDevice->EndScene();

	g_pDevice->Present();
}

///////////////////////////////////
// 도움말 출력
void ShowInfo()
{
	static bool bShow = true;
	if (IsKeyUp(VK_F1)) bShow ^= true;

	//PutFPS(1, 1);

	if (!bShow) return;

	{
		//int x = 350, y = 1;			
		int x = g_Mode.Width / 2 - 100;
		int y = 50;
		COLORREF col = RGB(255, 255, 255);
		COLORREF col2 = RGB(255, 255, 0);
		COLORREF col3 = RGB(150, 150, 0);
		COLORREF col4 = RGB(150, 150, 150);
		COLORREF col5 = RGB(200, 200, 200);
		DrawText(x, y, col, _T("■ %s"), g_WindowName);
		DrawText(x, y += 14, col3, _T("1. 정점(Vertex) 구성"));
		DrawText(x, y += 14, col3, _T("4. 삼각형(Face) 출력"));
		//DrawText(x, y += 14, col3, _T("2. 정점 규격(Vertex Format) 의 이해"));
		//DrawText(x, y += 14, col3, _T("3. 정점 버퍼(Vertex Buffer) 구축"));
		//DrawText(x, y += 14, col3, _T("4. 삼각형(Face) 출력 : Wireframe"));
		//DrawText(x, y += 14, col3, _T("5. 삼각형(Face) 출력 : Fill Mode"));
		DrawText(x, y += 15, col2, _T("6. 렌더링 상태 구현 : SetRenderState"));
		DrawText(x, y += 15, col3, _T("7. 채우기 모드(Fill Mode) 전환 : SPACE BAR"));
		DrawText(x, y += 15, col2, _T("8. 컬링 모드(Culling Mode) 전환 : F5"));


		y += 14 * 2;
		DrawText(x, y += 14, col5, _T("<다음 주제>"));
		DrawText(x, y += 14, col5, _T("* 정점 색상(Vertex Color) 처리"));
		DrawText(x, y += 14, col5, _T("* 픽셀 색상 계산 : 선형보간(Linear Interpolation)"));
		DrawText(x, y += 14, col5, _T("* 픽셀 색상 출력 : 레스터(Rasterization) 개정"));


		y += 14;
		DrawText(x, y += 14, RGB(0, 255, 255), _T("■ _DrawLine (GDI)"));
	}


	//좌측 도움말
	{
		int x = 1;
		int y = 100;
		COLORREF col = RGB(0, 255, 0);
		DWORD v;

		TCHAR* wmsg[] = { _T("N/A"), _T("POINT"), _T("WIRE"), _T("SOILD") };
		g_pDevice->GetRenderState(B3MRS_FILLMODE, &v);
		DrawText(x, y += 14, col, _T("Fill: Space (%s)"), wmsg[v]);

		TCHAR* cmsg[] = { _T("N/A"), _T("NONE"), _T("CW"), _T("CCW") };
		g_pDevice->GetRenderState(B3MRS_CULLMODE, &v);
		DrawText(x, y += 14, col, _T("Cull: F5 (%s)"), cmsg[v]);
	}
}



