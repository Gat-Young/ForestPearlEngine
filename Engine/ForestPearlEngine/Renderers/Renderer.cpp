#include "Renderer.h"
#include <Windows.h>
#include "mmsystem.h"
#include <string>
#include "tchar.h"
#include <iostream>

Renderer::Renderer()
{
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

	HDC hdc = nullptr;
	FPRenderDevice->GetDC(&hdc);
	SelectObject(hdc, g_hSysFont);

	return S_OK;
}

void Renderer::ObjectRendering(std::vector<FPActor*> RenderList)
{
	FPRenderDevice->BeginScene();
	FPRenderDevice->Clear(0, NULL, FPRHICLEAR_TARGET, FPRHICOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 1.0f), 1.0f, 0);
	FPRenderDevice->EndScene();

}

void Renderer::UIRendering(std::vector<FPActor*> UIList)
{
	for(FPActor* UI : UIList)
	{
		for (int i = 0; i < UI->UI_data.size(); ++i)
		{
			Renderer::DrawText(UI->UI_data[i]->x, UI->UI_data[i]->y, UI->UI_data[i]->color, UI->UI_data[i]->msg.c_str());
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
	SetTextColor(hdc, col);
	::DrawText(hdc, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(hdc, RGB(0, 255, 0));
}
