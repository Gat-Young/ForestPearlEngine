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
		FPRHIVertexBuffer* FPVertexBuffer = nullptr;

		HFONT		g_hSysFont = NULL;

		// 정점 포멧(VERTEX FORMAT) 
		struct COLVTX
		{
			float x, y, z;        //x(pos), y(pos), z(Depth)★
			float rhw;			  //동차변환성분 w의 역수, 즉 1/w : reciprocal homogeneous W
			DWORD color;
		};

		// 정점 포멧의 플래그 조합.. 
		DWORD FVF_COLVTX = (FPRHIFVF_XYZRHW | FPRHIFVF_DIFFUSE);

		COLVTX MakeCOLVTX(float x, float y, float z, float rhw, DWORD color);

		void DrawText(int x, int y, COLORREF color, const TCHAR* msg, ...);

	public:
		Renderer();

		HRESULT InitializeRenderer(UINT DeviceVersion, HWND hwnd);

		HRESULT MakeVB(std::vector<FPActor*> RenderList);

		void ObjectRendering(std::vector<FPActor*> RenderList);
		void UIRendering(std::vector<FPActor*> UIList);
		void RenderTargetPresent();

};

