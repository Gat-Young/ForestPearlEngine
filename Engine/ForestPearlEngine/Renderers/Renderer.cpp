#include "Renderer.h"
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
	return S_OK;
}

void Renderer::Rendering(std::vector<FPActor*> RenderList)
{
	FPRenderDevice->BeginScene();
	FPRenderDevice->Clear(0, NULL, FPRHICLEAR_TARGET, FPRHICOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 1.0f), 1.0f, 0);
	FPRenderDevice->EndScene();

	FPRenderDevice->Present(NULL, NULL, NULL, NULL);
}

