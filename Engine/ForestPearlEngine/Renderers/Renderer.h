#pragma once
#include "RenderingDevice.h"
#include "../Shader/Shader.h"

//정점 구조체
struct VERTEX
{
	float x, y, z;
};

class Renderer
{

	private:
		RenderingDevice& Device;

		//Font
		SpriteBatch* FontBatch = nullptr;
		SpriteFont* Font = nullptr;

		//기본 셰이더
		Shader* DefaultShader = nullptr;

		//폰트 해제
		void FontRelease();

	public:
		Renderer(RenderingDevice& Device);

		void ClearBackBuffer();

		void RenderTargetPresent();

		HRESULT InitializeRenderer(HWND hwnd);

		void ObjectRendering();

		void UIRendering();

		HRESULT Finalize();

		RenderingDevice& GetRenderingDevice();
};