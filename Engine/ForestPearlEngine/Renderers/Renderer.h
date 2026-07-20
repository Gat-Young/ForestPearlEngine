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

		//ºŒ¿Ã¥ı ∆—≈‰∏Æ
		ShaderFactory* shaderFactory = nullptr;

		//∆˘∆Æ «ÿ¡¶
		void FontRelease();

		//Gizmo Rendering
		void GizmoRendering(struct ConstBuffer& cb);

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