#pragma once

#include "RenderingDevice.h"
#include "../Shader/Shader.h"
using namespace DirectX;

//정점 구조체
struct VERTEX
{
	float x, y, z;
};

class Renderer
{

	private:
		std::unique_ptr<RenderingDevice> Device;

		SpriteBatch* FontBatch = nullptr;
		SpriteFont* Font = nullptr;

		//폰트 해제
		void FontRelease();

	public:
		Renderer();

		void ClearBackBuffer();

		void RenderTargetPresent();

		HRESULT InitializeRenderer(HWND hwnd);

		void ObjectRendering();

		void UIRendering();

		HRESULT Finalize();

		RenderingDevice* GetRenderingDevice();
};