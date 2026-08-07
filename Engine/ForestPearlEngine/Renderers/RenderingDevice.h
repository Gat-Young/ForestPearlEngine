#pragma once

#include <Windows.h>
#include <wrl/client.h>
#include <tchar.h>
#include <vector>

#include <d3d11.h>
#include <dxgi1_6.h>
#pragma comment(lib, "D3D11")
#pragma comment(lib, "dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#pragma comment(lib, "DirectXTK.lib")
#include "SpriteFont.h"
#include "SpriteBatch.h"
using namespace DirectX;

class RenderingDevice
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
		ComPtr<IDXGISwapChain1>			SwapChain = NULL;
		ComPtr<ID3D11RenderTargetView>	RenderTargetView = NULL;
		ComPtr<ID3D11Texture2D>			DepthStencilBuffer = NULL;
		ComPtr<ID3D11DepthStencilView>	DepthStencilBufferView = NULL;


		//Rasterizer 상태 객체
		enum {
			RS_SOLID,				//기본 렌더링 : 솔리드 Solid
			RS_WIREFRAME,			//와이어프레임 렌더링
			RS_CULLBACK,			//뒷면 컬링(ON) : BackFaceCulling - "CCW"
			RS_WIRECULLBACK,		//와이어 프레임 + 뒷면 컬링 (ON)

			RS_MAX_
		};

		//렌더링 모드 : 다수의 렌더링 모드 조합 및 운용을 위한 정의
		enum
		{
			RM_SOLID = 0x0000,			//삼각형채우기 : ON, Solid
			RM_WIREFRAME = 0x0001,		//삼각형채우기 : OFF, Wire-frame
			RM_CULLBACK = 0x0002,		//뒷면 컬링 : On "CCW"

			//렌더링 기본 모드 : Solid + Cull-On
			RM_DEFAULT = RM_SOLID | RM_CULLBACK,
		};
		DWORD RMode = RM_DEFAULT; //"현재 렌더링 모드"

		//Rasterizer 상태 객체 배열
		ID3D11RasterizerState* RState[RS_MAX_] = { NULL, };

		//깊이/스텐실 테스트 상태들
		enum {
			DS_DEPTH_ON,			//깊이버퍼 ON! (기본값), 스텐실버퍼 off
			DS_DEPTH_OFF,			//깊이버퍼 OFF
			DS_DEPTH_WRITE_OFF,		//깊이버퍼 쓰기 끄기

			DS_MAX_,
		};
		//깊이/스텐실 버퍼 상태 객체
		ID3D11DepthStencilState* DSState[DS_MAX_];

		DXGI_MODE_DESC1 DisplayMode;

		//AA & AF Option
		DWORD dwAA = 4;				//AA off 는 1로, AA 적용시 배수 지정 (최대 8)
		DWORD dwAF = 8;				//Anisotropic Filter 배수. ( 최대 16 )
		BOOL IsMipMap = TRUE;

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

		//ConstBuffer
		ID3D11Buffer* ConstBuffer;

		//VSync 여부
		bool IsVSync = true;

		//전체화면 여부
		bool IsFullScreen = false;

		const TCHAR* StrFeatureLevel = _T("N/A");

		XMFLOAT4 BackGroundColor = { 0, 0.125f, 0.3f, 1 };

	public:
		static RenderingDevice& GetRenderingDevice();

		HRESULT SetDisplayMode(HWND hwnd);

		HRESULT CreateFactory();

		HRESULT CreateHighSpecDevice();
		HRESULT CreateDevice(IDXGIAdapter1* Adapter, UINT DeviceFlags);

		HRESULT CreateSwapChain(HWND hwnd);

		HRESULT CreateDepthStencil();

		HRESULT SetBackBufferToRenderTargetView();

		HRESULT OMSetRenderTargets();

		HRESULT SetViewPort(float TopLeftX, float TopLeftY, float Width, float Height, float MinDepth, float MaxDepth);

		void ClearBackBuffer();

		void RenderTargetPresent();

		HRESULT DeviceFinalize();

		HRESULT Draw(UINT VertexCount, UINT StartVertexLocation);

		//VB 만들기
		void* CreateVertexBuffer(void* VertexData, UINT Size, UINT Stride);

		//CB 만들기
		int CreateConstBuffer(UINT Size);
		int CreateConstBuffer(UINT Size, ID3D11Buffer** ReturnConstBuffer);

		//CB 등록
		HRESULT VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers);

		//CB 업데이트
		HRESULT UpdateSubresource(UINT DstSubresource, void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);

		//입력 레이아웃 생성
		HRESULT CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* Ed, DWORD Num, ID3DBlob* InVSCode, ID3D11InputLayout** ReturnLayout);

		//깊이 스텐실 버퍼 상태객체 생성
		HRESULT CreateDepthStencilStateCreate();

		//깊이 스텐실 버퍼 상태 설정
		void OMSetDepthStencilState(bool State);

		//GetDXDevice
		ID3D11Device* GetDXDevice() { return Device.Get(); };

		//GetDXDeviceContext
		ID3D11DeviceContext* GetDXDeviceContext() { return DeviceContext.Get(); };

		//폰트 생성
		SpriteFont* CreateSpriteFont();

		SpriteBatch* CreateSpriteBatch();

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

		//레스터라이저 상태 객체 생성
		void RasterStateCreate();

		//레스터라이저 상태 객체 제거
		void RasterStateRelease();


		//렌더링스테이트 업데이트 함수
		void UpdateRSSetState(bool isFill, bool isCull);

		//셰이더 설정
		void VSSetShader(void* VS);
		void PSSetShader(void* PS);

		//정점 버퍼 설정
		void IASetVertexBuffers(UINT StartSlot, UINT NumBuffers, void* VertexBuffer, UINT* Strides, UINT* Offsets);
		
		//입력 레이아웃 설정
		void IASetInputLayout(void* InputLayout);

		//기하 위상 구조 설정
		void IASetPrimitiveTopology(enum Topology topo);

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