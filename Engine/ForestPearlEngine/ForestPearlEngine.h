#pragma once
#include "framework.h"
#include <vector>
#include "FPGameInstance.h"


class FPObject;
class FPActor;
class Renderer;
class RenderingDevice;

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

	//렌더러 정보 제공
	public:
		//장치 정보 반환 함수
		const TCHAR* GetAdapterDescription(int index);
		UINT GetAdapterVendorID(int index);
		UINT GetAdapterDeviceID(int index);
		UINT GetAdapterSubSysID(int index);
		UINT GetAdapterRevision(int index);
		SIZE_T GetAdapterVideoMem(int index);
		SIZE_T GetAdapterSystemMem(int index);
		SIZE_T GetAdapterSharedSysMem(int index);
		LONG GetAdapterLuidHighPart(int index);
		DWORD GetAdapterLuidLowPart(int index);

		//모니터 정보 반환
		const TCHAR* GetMonitorName(int AdapterIndex, int MonitorIndex);
		RECT GetDesktopCoordinates(int AdapterIndex, int MonitorIndex);

		//VRAM 정보 반환
		double GetVRAMBudget(int AdapterIndex);
		double GetVRAMCurrUsage(int AdapterIndex);
		double GetVRAMAvailableForReservation(int AdapterIndex);
		double GetVRAMCurrReservation(int AdapterIndex);

		//장치 개수 반환
		int GetAdapterSize();

		//장치의 모니터 개수 반환
		int GetAdapterMonitorSize(int index);


		const TCHAR* GetSrtFeatureLevel();
		UINT GetWidth();
		UINT GetHeight();

		//깊이 스텐실 버퍼 설정
		void SetZEnable(bool State);

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

		////////////////////////////////
		// RenderingDevice
		RenderingDevice* RenderDevice;

		// 나중에 설정파일 로더로 변경할 것
		////////////////////////////////
		// Window Property
		const wchar_t* WinClassName = L"MyFirstWndGame";
		const wchar_t* WinName = L"MyFirstWndGame";
		const int WinWidth = 960;
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

