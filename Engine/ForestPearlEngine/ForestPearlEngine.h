#pragma once
#include "framework.h"
#include <vector>
#include "FPGameInstance.h"


class FPObject;
class FPActor;
class Renderer;

class ForestPearlEngine
{

	public:
		static ForestPearlEngine& GetGameEngine();
		bool PreInitialize();
		bool Initialize();
		void GameLoop();
		void Finalize();

	//엔진 기능 Function
	public:
		void StopEngine();

	private:
		ForestPearlEngine() = default;

		HWND CreateFPEWindow(const wchar_t* className, const wchar_t* windowName, const int width, const int height);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		int MessagePump();

	private:
		////////////////////////////////
		// Engine Property
		bool bEngineLoop = true;

		////////////////////////////////
		// Game Property

		////////////////////////////////
		// Renderer
		Renderer* Render;

		// 나중에 설정파일 로더로 변경할 것
		////////////////////////////////
		// Window Property
		const wchar_t* WinClassName = L"MyFirstWndGame";
		const wchar_t* WinName = L"MyFirstWndGame";
		const int WinWidth = 800;
		const int WinHeight = 600;

		////////////////////////////////
		// Render Property 
		// Window
		HWND Hwnd = nullptr;
		HDC BackHdc = nullptr;
		HDC FrontHdc = nullptr;
		HBITMAP BackBitmap = nullptr;
		HBITMAP DefaultBitmap = nullptr;

		////////////////////////////////
		// Input System 
		private:
			void RegisterFPRawInputDevices();
};

