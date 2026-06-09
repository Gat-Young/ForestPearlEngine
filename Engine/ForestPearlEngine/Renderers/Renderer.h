#pragma once
#include <vector>
#include "../Object/Actor.h"
#include "FPRHI/FPRHI.h"

// 정점 포멧(VERTEX FORMAT) 
struct COLVTX
{
	float x, y, z;        //x(pos), y(pos), z(Depth)★
	float rhw;			  //동차변환성분 w의 역수, 즉 1/w : reciprocal homogeneous W
	DWORD color;
};

COLVTX MakeCOLVTX(float x, float y, float z, float rhw, DWORD color);

class Renderer
{
	private:
		FPRHI* FPRender = nullptr;
		FPRHIDevice* FPRenderDevice = nullptr;
		FPRHIDISPLAYMODE FPDisplayMode;
		std::vector<FPRHIVertexBuffer*> FPVertexBufferList;
		int VertexBufferSize = -1;

		HFONT		g_hSysFont = NULL;

		// 정점 포멧의 플래그 조합.. 
		DWORD FVF_COLVTX = (FPRHIFVF_XYZRHW | FPRHIFVF_DIFFUSE);

		void DrawText(int x, int y, COLORREF color, const TCHAR* msg, ...);

	public:
		Renderer();

		HRESULT InitializeRenderer(UINT DeviceVersion, HWND hwnd);

		int MakeVB(std::vector<COLVTX> Vertex);

		void ObjectRendering();
		void UIRendering(std::vector<FPActor*> UIList);
		void RenderTargetPresent();

};

