#pragma once

#include <d3d11.h>
#include <dxgi1_6.h>
#pragma comment(lib, "D3D11")
#pragma comment(lib, "dxgi.lib")

#include <Windows.h>
#include <wrl/client.h>
#include <tchar.h>
#include <DirectXMath.h>
#pragma comment(lib, "DirectXTK.lib")
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include <vector>

#include "../Shader/Shader.h"
using namespace DirectX;

//정점 구조체
struct VERTEX
{
	float x, y, z;
};

class Renderer
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	private:

		//D3D Factory
		ComPtr<IDXGIFactory2> Factory = NULL;

		//D3D 장치 인터페이스
		ComPtr<ID3D11Device> Device = NULL;
		ComPtr<ID3D11DeviceContext> DeviceContext = NULL;

		//D3D 스왑체인 인터페이스
		ComPtr<IDXGISwapChain1> SwapChain = NULL;
		ComPtr<ID3D11RenderTargetView> RenderTargetView = NULL;

		DXGI_MODE_DESC1 DisplayMode;

		//D3D Feature Level 확인
		D3D_FEATURE_LEVEL FeatureLevels[2] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL ActualLevel;


		//현재 하드웨어 정보 관리 구조체
		struct DEVICEINFO
		{
			UINT Index;
			DXGI_ADAPTER_DESC1 AdapterDescription;		//어댑터 정보
			DXGI_QUERY_VIDEO_MEMORY_INFO VRAMInfo;		//어댑터의 VRAM 정보

			std::vector<DXGI_OUTPUT_DESC> OuputDesc;	//모니터 정보
		};
		std::vector<DEVICEINFO> DevInfo;				//다중 GPU를 위한 배열 처리

		//VSync 여부
		bool IsVSync = true;

		//전체화면 여부
		bool IsFullScreen = false;

		const TCHAR* StrFeatureLevel = _T("N/A");



		XMFLOAT4 BackGroundColor = { 0, 0.125f, 0.3f, 1 };
		
		//폰트 정보
		SpriteBatch* FontBatch = nullptr;
		SpriteFont* Font = nullptr;

		//뷰포트 세팅
		void SetViewPort(float TopLeftX, float TopLeftY, float Width, float Height, float MinDepth, float MaxDepth);
		
		//폰트 생성
		HRESULT FontCreate();

		//폰트 해제
		void FontRelease();

		//장치 정보 획득
		void GetDeviceInfo();

		//장치 기능레벨 확인
		void GetFeatureLevel();

		//GPU 정보 얻기
		HRESULT GetAdapterInfo();

		//모니터 정보 획득
		HRESULT GetDXMonitorInfo(IDXGIAdapter1* Adapter, DEVICEINFO& Di);

		//VRAM 정보 획득
		HRESULT GetDXVRAMInfo(IDXGIAdapter1* Adapter, DEVICEINFO& Di);


	public:
		Renderer();

		HRESULT InitializeRenderer(HWND hwnd);

		void ClearBackBuffer();

		void ObjectRendering();

		void UIRendering();

		void RenderTargetPresent();

		HRESULT Finalize();


		//장치 정보 반환 함수
		const TCHAR* GetAdapterDescription(int index) { return DevInfo[index].AdapterDescription.Description; };
		UINT GetAdapterVendorID(int index) { return DevInfo[index].AdapterDescription.VendorId; };
		UINT GetAdapterDeviceID(int index) { return DevInfo[index].AdapterDescription.DeviceId; };
		UINT GetAdapterSubSysID(int index) { return DevInfo[index].AdapterDescription.SubSysId; };
		UINT GetAdapterRevision(int index) { return DevInfo[index].AdapterDescription.Revision; };
		#define ToMB(a) (a/1024.0/1024.0)
		SIZE_T GetAdapterVideoMem(int index) { return ToMB(DevInfo[index].AdapterDescription.DedicatedVideoMemory); };
		SIZE_T GetAdapterSystemMem(int index) { return ToMB(DevInfo[index].AdapterDescription.DedicatedSystemMemory); };
		SIZE_T GetAdapterSharedSysMem(int index) { return ToMB(DevInfo[index].AdapterDescription.SharedSystemMemory); };
		LONG GetAdapterLuidHighPart(int index) { return DevInfo[index].AdapterDescription.AdapterLuid.HighPart; };
		DWORD GetAdapterLuidLowPart(int index) { return DevInfo[index].AdapterDescription.AdapterLuid.LowPart; };

		//모니터 정보 반환
		const TCHAR* GetMonitorName(int AdapterIndex, int MonitorIndex) { return DevInfo[AdapterIndex].OuputDesc[MonitorIndex].DeviceName; };
		RECT GetDesktopCoordinates(int AdapterIndex, int MonitorIndex) { return DevInfo[AdapterIndex].OuputDesc[MonitorIndex].DesktopCoordinates; };

		//VRAM 정보 반환
		double GetVRAMBudget(int AdapterIndex) { return (double)ToMB(DevInfo[AdapterIndex].VRAMInfo.Budget); }
		double GetVRAMCurrUsage(int AdapterIndex) { return (double)ToMB(DevInfo[AdapterIndex].VRAMInfo.CurrentUsage); }
		double GetVRAMAvailableForReservation(int AdapterIndex) { return (double)ToMB(DevInfo[AdapterIndex].VRAMInfo.AvailableForReservation); }
		double GetVRAMCurrReservation(int AdapterIndex) { return (double)ToMB(DevInfo[AdapterIndex].VRAMInfo.CurrentReservation); }
		
		//장치 개수 반환
		int GetAdapterSize() { return DevInfo.size(); };
		//장치의 모니터 개수 반환
		int GetAdapterMonitorSize(int index) { return DevInfo[index].OuputDesc.size(); };

		const TCHAR* GetSrtFeatureLevel() { return StrFeatureLevel; };
		UINT GetWidth() { return DisplayMode.Width; };
		UINT GetHeight() { return DisplayMode.Height; };
};