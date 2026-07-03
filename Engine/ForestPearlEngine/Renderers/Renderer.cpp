#include "Renderer.h"
#include <assert.h>
#include <iostream>

Renderer::Renderer()
{
}

HRESULT Renderer::InitializeRenderer(HWND hwnd)
{
	HRESULT hr = S_OK;

	//DXGI Display 모드 설정
	RECT rect;
	GetClientRect(hwnd, &rect);
	int BackBufferWidth = rect.right - rect.left;
	int BackBufferHeight = rect.bottom - rect.top;

	DisplayMode.Width = BackBufferWidth;
	DisplayMode.Height = BackBufferHeight;
	DisplayMode.RefreshRate = {0, 1};
	DisplayMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;


	//DXGI Factory 생성
	UINT DXGIFactoryFlags = 0;
#if defined(_DEBUG) || !defined(NDEBUG)
	DXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	hr = CreateDXGIFactory2(DXGIFactoryFlags, IID_PPV_ARGS(Factory.GetAddressOf()));
	assert(SUCCEEDED(hr));
	assert(Factory != nullptr && "Failed To Create DXGI Factory\n");

	//현재 사용중인 Adapter(GPU)를 찾고 D3D11을 지원하는 지 확인
	ComPtr<IDXGIAdapter1> Adapter, SelectedAdapter;
	SIZE_T MaxDedicatedMemory = 0;

	for (UINT i = 0; Factory->EnumAdapters1(i, &Adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 Desc;
		Adapter->GetDesc1(&Desc);

		//CPU 기반 소프트웨어 렌더러라면 무시
		if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

		//Adapter가 DX11을 지원하는지 확인
		if (SUCCEEDED(D3D11CreateDevice(
			Adapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			nullptr,
			nullptr,
			nullptr)))
		{
			//가장 비디오 메모리가 큰 Adapter로 선택
			if (Desc.DedicatedVideoMemory > MaxDedicatedMemory)
			{
				MaxDedicatedMemory = Desc.DedicatedVideoMemory;
				SelectedAdapter = Adapter;
			}
		}
	}
	assert(SelectedAdapter == nullptr && "No Compatible DX11 Adapter Found");
	
	//색상 Format 설정
	UINT DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG) || !defined(NDEBUG)
	DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//D3D Device 생성
	hr = D3D11CreateDevice(
		NULL,						//비디오 어댑터 포인터 (기본장치는 NULL)
		D3D_DRIVER_TYPE_HARDWARE,	//HW 가속
		nullptr,					//SW Resterizer DLL 핸들, HW 가속시에는 NULL
		DeviceFlags,				//디바이스 생성 플래그. (기본값)
		FeatureLevels,				//(생성할) 디바이스 기능 레벨(Feature Level) 배열
		_countof(FeatureLevels),	//(생성할) 디바이스 기능 레벨(Feature Level) 배열 크기
		D3D11_SDK_VERSION,			//DX SDK 버전.
		Device.GetAddressOf(),		//[출력] 디바이스 인터페이스 얻기
		&ActualLevel,				//[출력] (생성된) 디바이스 기능 레벨. 필요없다면 NULL 설정.
		nullptr						//[출력] 디바이스 컨텍스트 얻기. 필요없다면 NULL 설정.
	);
	assert(Device != nullptr && "Failed To Create DX11 Device");

	Device->GetImmediateContext(&DeviceContext);	//현재 디바이스 컨텐스트 얻기

	assert(SUCCEEDED(hr) && "디바이스 / 스왑체인 생성 실패\n");

	//Swap Chain 생성
	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
	SwapChainDesc.Width = DisplayMode.Width;		//해상도 결정(백버퍼 크기)
	SwapChainDesc.Height = DisplayMode.Height;
	SwapChainDesc.Format = DisplayMode.Format;		//백버퍼 색상규격
	SwapChainDesc.Stereo = FALSE;					//스테레오 3D렌더링 옵션
	SwapChainDesc.SampleDesc = { 1, 0 };			//멀티 샘플링 설정 {Count, Quality}
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//용도 설정 : 렌더 타겟
	SwapChainDesc.BufferCount = 1;					//백버퍼 개수
	SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;	//출력 화면 크기가 모니터의 실제 해상도와 다를 때 어떻게 스케일링 할지 여부


	return hr;
}
