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

	public:
		Renderer();
		HRESULT InitializeRenderer(UINT DeviceVersion, HWND hwnd);
		void Rendering(std::vector<FPActor*> RenderList);
};

