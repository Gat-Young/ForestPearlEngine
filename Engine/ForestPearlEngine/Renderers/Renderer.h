#pragma once
#include "RenderingDevice.h"
#include "../Shader/ShaderFactory.h"

//정점 구조체
struct VERTEX
{
	float x, y, z;		//좌표 Position
	float r, g, b, a;	//색상 Diffuse Color
};

class Renderer
{

	private:
		RenderingDevice& Device;

		//VertexBuffer List


		//Font
		SpriteBatch* FontBatch = nullptr;
		SpriteFont* Font = nullptr;

		//셰이더 팩토리
		ShaderFactory* shaderFactory = nullptr;

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