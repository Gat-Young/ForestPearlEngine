#pragma once
#include <vector>
#include "FPRHI/FPRHI.h"

// 정점 포멧(VERTEX FORMAT) 
struct COLVTX
{
	float x, y, z;        //x(pos), y(pos), z(Depth)★
	DWORD color;
};

COLVTX MakeCOLVTX(float x, float y, float z, DWORD color);

class Legacy_Renderer
{
	private:
		FPRHI* FPRender = nullptr;
		FPRHIDevice* FPRenderDevice = nullptr;
		FPRHIDISPLAYMODE FPDisplayMode;
		std::vector<FPRHIVertexBuffer*> FPVertexBufferList;
		int VertexBufferSize = -1;

		HFONT		g_hSysFont = NULL;

		// 정점 포멧의 플래그 조합.. 
		DWORD FVF_COLVTX = (FPRHIFVF_XYZ | FPRHIFVF_DIFFUSE);

		void DrawText(int x, int y, COLORREF color, const TCHAR* msg, ...);

	public:
		Legacy_Renderer();

		HRESULT InitializeRenderer(UINT DeviceVersion, HWND hwnd);

		int MakeVB(std::vector<COLVTX> Vertex);

		void ObjectRendering();
		void UIRendering();
		void RenderTargetPresent();

};

