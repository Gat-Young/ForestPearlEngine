#pragma once
#include "../Renderer.h"
#include <windows.h>

class SimpleGDI : public Renderer
{
	public:
		SimpleGDI(HWND hwnd);
		virtual void Rendering(std::vector<FPActor*> RenderList) override;
	
	private:
		////////////////////////////////
		// Render Property 
		// Window
		HWND Hwnd = nullptr;
		HDC BackHdc;
		HDC FrontHdc;
		HBITMAP BackBitmap = nullptr;
		HBITMAP DefaultBitmap = nullptr;

		int WinWidth;
		int WinHeight;

		///////////////////////////////
		// Rendering ÇÔ¼ö µé
		void DrawCollider(HDC hdc, FPActor* Actor);
};

