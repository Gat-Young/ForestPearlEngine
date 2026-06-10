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
	float x, y, z;		// 정점 좌표
	DWORD diff;     // 정점 색
};

// 정점 포멧의 플래그 조합
#define FVF_COLVTX (B3MFVF_XYZ | B3MFVF_DIFFUSE)

// 렌더링 상태 변수
BOOL g_bWireFrame = FALSE;  // 와이어 프레임 출력 플래그
BOOL g_bCulling = FALSE;    // 뒷면제거 플래그

B3MXCOLOR	g_ClearColor(0.35f, 0.35f, 0.35f, 1.0f);		//배경색

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
		{  -0.5f, 0.0f, 0.0f,  0xffff0000 },
		{   0.0f, 1.0f, 0.0f,  0xff00ff00 },
		{   0.5f, 0.0f, 0.0f,  0xff00ffff },

		{  -0.5f, 0.0f, 0.0f,  0xffff0000 },
		{   0.0f,-1.0f, 0.0f,  0xff00ff00 },
		{   0.5f, 0.0f, 0.0f,  0xff00ffff },
	};

	if (MC_FAILED(g_pDevice->CreateVertexBuffer(sizeof(Vertices), 0, FVF_COLVTX, B3MPOOL_SYSTEMMEM, &g_pVB, NULL)))
	{
		return MC_FALSE;
	}

	COLVTX* pBuff = nullptr;
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

	g_pDevice->DrawPrimitive(B3MPT_TRIANGLELIST, 0, 2);

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

	// 렌더링 모드에 따른 배경색 설정
	if (g_bWireFrame) g_ClearColor = B3MXCOLOR(0.20f, 0.20f, 0.20f, 1.0f);
	else	g_ClearColor = B3MXCOLOR(0.0f, 0.125f, 0.35f, 1.0f);
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

	g_pDevice->Clear(g_ClearColor);

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
		B3MXCOLOR col = RGB(255, 255, 255);
		B3MXCOLOR col2 = RGB(255, 255, 0);
		B3MXCOLOR col3 = RGB(150, 150, 0);
		B3MXCOLOR col4 = RGB(150, 150, 150);
		B3MXCOLOR col5 = RGB(200, 200, 200);
		B3MXCOLOR col7(0, 1, 1, 1);

		DrawText(x, y, col, _T("■ %s"), g_WindowName);
		y += 14;
		DrawText(x, y += 14, col2, _T("1. 렌더링 파이프라인(Rendering Pipeline) 의 이해."));
		DrawText(x, y += 14, col2, _T("2. 3D->2D 변환 과정의 이해 및 연구."));
		DrawText(x, y += 14, col2, _T("3. Local (Model) Space 의 이해."));
		y += 14;
		DrawText(x, y += 14, col2, _T("* 정점 파이프라인 (Vertex Pipeline) 구현."));
		DrawText(x, y += 14, col2, _T("* 기하 파이프라인 (Geometry Pipeline) 구현."));
		DrawText(x, y += 14, col2, _T("* 픽셀 파이프라인 (Pixel Pipeline) 구현."));

		//y += 14;
		y = g_Mode.Height - 100;
		DrawText(x, y += 14, col7, _T("* _DrawFace (GDI)"));
	}


	//좌측 도움말
	{
		int x = 1;
		int y = 100;
		B3MXCOLOR col(0, 1, 0, 1);
		DWORD v;

		TCHAR* wmsg[] = { _T("N/A"), _T("POINT"), _T("WIRE"), _T("SOILD") };
		g_pDevice->GetRenderState(B3MRS_FILLMODE, &v);
		DrawText(x, y += 14, col, _T("Fill: Space (%s)"), wmsg[v]);

		TCHAR* cmsg[] = { _T("N/A"), _T("NONE"), _T("CW"), _T("CCW") };
		g_pDevice->GetRenderState(B3MRS_CULLMODE, &v);
		DrawText(x, y += 14, col, _T("Cull: F5 (%s)"), cmsg[v]);
	}
}



