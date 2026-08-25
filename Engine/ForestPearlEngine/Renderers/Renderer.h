#pragma once
#include "RenderingDevice.h"
#include "../Shader/ShaderFactory.h"

class Renderer
{

	private:
		RenderingDevice& Device;

		//Font
		SpriteBatch* FontBatch = nullptr;
		SpriteFont* Font = nullptr;

		//셰이더 팩토리
		ShaderFactory* shaderFactory = nullptr;

		//깊이 연산 모드 전환 값
		bool ZEnable = true;

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

		//RenderTarget 재생성 (임시)
		void ResizeRenderTarget();

		//렌더링 속성 변경
		void SetZEnable(bool State) { ZEnable = State; };
};