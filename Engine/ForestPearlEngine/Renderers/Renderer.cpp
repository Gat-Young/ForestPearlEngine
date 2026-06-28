#include "Renderer.h"
#include <Windows.h>
#include "mmsystem.h"
#include <string>
#include "tchar.h"
#include <iostream>
#include <stack>
#include "../MeshRenderList.h"
#include "../TextRenderList.h"
#include "../CameraList.h"
#include "../GizmoRenderList.h"

Renderer::Renderer()
{
}

COLVTX MakeCOLVTX(float x, float y, float z, DWORD color)
{
	return COLVTX{ x, y, z, color };
}

HRESULT Renderer::InitializeRenderer(UINT DeviceVersion, HWND hwnd)
{
	FPRender = CreateRHI(DeviceVersion);

	//디스플레이 설정
	FPDisplayMode.Width = 800;
	FPDisplayMode.Height = 600;
	FPDisplayMode.RefreshRate = 0;
	FPDisplayMode.Format = FPRHIFMT_A8R8G8B8;

	//Render Target 설정
	FPRHIPRESENT_PARAMETERS FPPresentParameters;
	ZeroMemory(&FPPresentParameters, sizeof(FPPresentParameters));
	FPPresentParameters.Windowed = TRUE;
	FPPresentParameters.BackBufferWidth = FPDisplayMode.Width;
	FPPresentParameters.BackBufferHeight = FPDisplayMode.Height;
	FPPresentParameters.BackBufferFormat = FPDisplayMode.Format;
	FPPresentParameters.BackBufferCount = 1;
	FPPresentParameters.SwapEffect = FPRHISWAPEFFECT_DISCARD;
	FPPresentParameters.PresentationInterval = FPRHIPRESENT_INTERVAL_IMMEDIATE;
	FPPresentParameters.Flags = FPRHIPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//Device 생성
	HRESULT res = FPRender->CreateDevice(
											FPRHIADAPTER_DEFAULT,							//0번 비디오 어뎁터.
											FPRHIDEVTYPE_HAL,								//하드웨어 레스터(HW Rasterization) 
											hwnd,											//생성할 윈도 핸들.
											FPRHICREATE_HARDWARE_VERTEXPROCESSING,			//정점 처리 방법.(GPU)
											&FPPresentParameters,							//화면 설정 '옵션'
											&FPRenderDevice									//생성된 장치의 포인터를 받을 포인터변수.
										);

	//폰트 생성 및 설정
	g_hSysFont = CreateFont(
		12, 6,
		0, 0, 1, 0, 0, 0,
		DEFAULT_CHARSET,	//HANGUL_CHARSET  
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE,
		_T("굴림")
	);
	return S_OK;
}

int Renderer::MakeVB(std::vector<COLVTX> Vertex)
{
	FPVertexBufferList.push_back(nullptr);
	VertexBufferSize++;

	//정점 버퍼 생성.
	if (FAILED(FPRenderDevice->CreateVertexBuffer(
		(Vertex.size() * sizeof(COLVTX)),				//'정점 버퍼'의 크기 (바이트)
		0,												// 버퍼 처리 유형 
		FVF_COLVTX,										//'정점' 스타일 
		FPRHIPOOL_MANAGED,								// 정점버퍼의 위치...MANAGED 추천.
		&FPVertexBufferList[VertexBufferSize],								// 성공시 리턴되는 버퍼 포인터ㅣ
		NULL											// 예약됨. 그냥 NULL.
	)))
	{
		return E_FAIL;
	}

	//버퍼 채우기. 
	VOID* pVB = nullptr;
	if (FAILED(FPVertexBufferList[VertexBufferSize]->Lock(0, (Vertex.size() * sizeof(COLVTX)), (void**)&pVB, 0)))
	{
		return E_FAIL;
	}
	memcpy(pVB, Vertex.data(), (Vertex.size() * sizeof(COLVTX)));
	FPVertexBufferList[VertexBufferSize]->Unlock();

	return VertexBufferSize;
}

void Renderer::ObjectRendering()
{
	std::vector<CameraItem> CamList = CameraList::Get().GetRenderList();

	for (CameraItem CamItem : CamList)
	{
		if (!*(CamItem.Active)) continue;

		FPRHITRANSFORMMATRIX g_mTM; //카메라 행렬

		g_mTM.position_x = CamItem.Position->x;
		g_mTM.position_y = CamItem.Position->y;
		g_mTM.position_z = CamItem.Position->z;
		g_mTM.position_w = 1.0f;

		g_mTM.rotation_x = CamItem.Rotation->x;
		g_mTM.rotation_y = CamItem.Rotation->y;
		g_mTM.rotation_z = CamItem.Rotation->z;
		g_mTM.rotation_z = 1.0f;

		g_mTM.scale_x = CamItem.Scale->x;
		g_mTM.scale_y = CamItem.Scale->y;
		g_mTM.scale_z = CamItem.Scale->z;
		g_mTM.scale_w = 1.0f;
		
		g_mTM.LookAt_x = CamItem.LookAt->x;
		g_mTM.LookAt_y = CamItem.LookAt->y;
		g_mTM.LookAt_z = CamItem.LookAt->z;
		g_mTM.LookAt_w = 1.0f;

		g_mTM.Up_x = CamItem.Up->x;
		g_mTM.Up_y = CamItem.Up->y;
		g_mTM.Up_z = CamItem.Up->z;
		g_mTM.Up_w = 1.0f;

		g_mTM.Fov = *(CamItem.Fov);
		g_mTM.Aspect = *(CamItem.Aspect);
		g_mTM.Zn = *(CamItem.Zn);
		g_mTM.Zf = *(CamItem.Zf);


		FPRenderDevice->SetTransform(FPRHITS_VIEW, &g_mTM);

		FPRenderDevice->SetTransform(FPRHITS_PROJECTION, &g_mTM);
	}

	FPRenderDevice->BeginScene();
	FPRenderDevice->Clear(0, NULL, FPRHICLEAR_TARGET, FPRHICOLOR_COLORVALUE(0, 0.12f, 0.35f, 1.0f), 1.0f, 0);

	std::vector<GizmoRenderItem> GizemoRenderList = GizmoRenderList::Get().GetRenderList();
	for (GizmoRenderItem RenderItem : GizemoRenderList)
	{
		if (!*(RenderItem.Active)) continue;

		//조명 끄기
		FPRenderDevice->SetRenderState(FPRHIRS_LIGHTING, FALSE);

		//출력 스트림 설정
		FPRenderDevice->SetStreamSource(0, FPVertexBufferList[*(RenderItem.VBIndex)], 0, sizeof(COLVTX));

		//정점 형식 설정
		FPRenderDevice->SetFVF(FVF_COLVTX);

		FPRHITRANSFORMMATRIX g_mTM; //변환 행렬

		g_mTM.position_x = RenderItem.Position->x;
		g_mTM.position_y = RenderItem.Position->y;
		g_mTM.position_z = RenderItem.Position->z;

		g_mTM.rotation_x = RenderItem.Rotation->x;
		g_mTM.rotation_y = RenderItem.Rotation->y;
		g_mTM.rotation_z = RenderItem.Rotation->z;

		g_mTM.scale_x = RenderItem.Scale->x;
		g_mTM.scale_y = RenderItem.Scale->y;
		g_mTM.scale_z = RenderItem.Scale->z;

		//월드 변환 행렬 설정 : 렌더링 전에 설정 되어야 합니다.
		FPRenderDevice->SetTransform(FPRHITS_WORLD, &g_mTM);		//★ 

		//기즈모 데이터 그리기
		FPRenderDevice->DrawPrimitive(FPRHIPT_LINELIST, 0, *(RenderItem.LineCount));    //Face 그리기
	}

	std::vector<MeshRenderItem> RenderList = MeshRenderList::Get().GetRenderList();
	/*std::cout << RenderList[0].Location->x << " : " << RenderList[0].Location->y << " : " << RenderList[0].Location->z << "\n"
		<< RenderList[0].Rotation->x << " : " << RenderList[0].Rotation->y << " : " << RenderList[0].Rotation->z << "\n"
		<< RenderList[0].Scale->x << " : " << RenderList[0].Scale->y << " : " << RenderList[0].Scale->z << "\n\n\n";*/
	for (MeshRenderItem RenderItem : RenderList)
	{
		//조명 끄기
		FPRenderDevice->SetRenderState(FPRHIRS_LIGHTING, FALSE);

		//렌더링 옵션 설정
		FPRenderDevice->SetRenderState(FPRHIRS_CULLMODE, *(RenderItem.isCull) ? FPRHICULL_CCW : FPRHICULL_NONE);
		//FPRenderDevice->SetRenderState(FPRHIRS_CULLMODE, FPRHICULL_CW);
		//FPRenderDevice->SetRenderState(FPRHIRS_CULLMODE, FPRHICULL_CCW);

		FPRenderDevice->SetRenderState(FPRHIRS_FILLMODE, *(RenderItem.isFill) ? FPRHIFILL_SOLID : FPRHIFILL_WIREFRAME);
	


		//출력 스트림 설정
		FPRenderDevice->SetStreamSource(0, FPVertexBufferList[*(RenderItem.VBIndex)], 0, sizeof(COLVTX));

		//정점 형식 설정
		FPRenderDevice->SetFVF(FVF_COLVTX);

		FPRHITRANSFORMMATRIX g_mTM; //변환 행렬

		g_mTM.position_x = RenderItem.Location->x;
		g_mTM.position_y = RenderItem.Location->y;
		g_mTM.position_z = RenderItem.Location->z;

		g_mTM.rotation_x = RenderItem.Rotation->x;
		g_mTM.rotation_y = RenderItem.Rotation->y;
		g_mTM.rotation_z = RenderItem.Rotation->z;

		g_mTM.scale_x = RenderItem.Scale->x;
		g_mTM.scale_y = RenderItem.Scale->y;
		g_mTM.scale_z = RenderItem.Scale->z;

		

			//월드 변환 행렬 설정 : 렌더링 전에 설정 되어야 합니다.
		FPRenderDevice->SetTransform(FPRHITS_WORLD, &g_mTM);		//★ 

		//기하데이터 그리기
		FPRenderDevice->DrawPrimitive(FPRHIPT_TRIANGLELIST, 0, *(RenderItem.FaceSize));    //Face 그리기
	}

	FPRenderDevice->EndScene();
}

void Renderer::UIRendering()
{
	std::vector<UIContextItem> RenderList = TextRenderList::Get().GetRenderList();
	for(UIContextItem UI : RenderList)
	{
		if (*(*(UI.active)))
		{
			Renderer::DrawText(*(UI.x), *(UI.y), *(UI.color), (*(UI.msg)).c_str());
		}
	}
}

void Renderer::RenderTargetPresent()
{
	FPRenderDevice->Present(NULL, NULL, NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
//
// 문자열 출력 (GDI)
//
// \param	x, y	출력 화면 좌표.
// \param	msg		출력 문자열 (형식화 문자열 지원)
// \return	없음.
//
//
void Renderer::DrawText(int x, int y, COLORREF col, const TCHAR* msg, ...)
{
	TCHAR buff[2048] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, _countof(buff), msg, vl);
	va_end(vl);
	RECT rc = { x, y, (LONG)(x + FPDisplayMode.Width), (LONG)(y + FPDisplayMode.Height) };

	HDC hdc = nullptr;
	FPRenderDevice->GetDC(&hdc);
	SelectObject(hdc, g_hSysFont);
	SetTextColor(hdc, col);
	SetBkMode(hdc, TRANSPARENT);
	::DrawText(hdc, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(hdc, RGB(0, 255, 0));
	FPRenderDevice->ReleaseDC(hdc);
}
