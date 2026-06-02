#pragma once
#include <vector>
#include "../Object/Actor.h"
#include "FPRHI/FPRHI.h"

class Renderer
{
	private:
		FPRHI* FPRender = nullptr;
		FPRHIDevice* FPRenderDevice = nullptr;
		FPRHIDISPLAYMODE FPDisplayMode;

		HFONT		g_hSysFont = NULL;

		void DrawText(int x, int y, COLORREF color, const TCHAR* msg, ...);

	public:
		Renderer();
		HRESULT InitializeRenderer(UINT DeviceVersion, HWND hwnd);
		void ObjectRendering(std::vector<FPActor*> RenderList);
		void UIRendering(std::vector<FPActor*> UIList);
		void RenderTargetPresent();
		void PutFPS(int x, int y);

};

