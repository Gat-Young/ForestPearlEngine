#include "RenderingDevice.h"
#include <assert.h>
#include <iostream>

//객체 해제/제거 매크로()
#ifndef SafeRelease
template<typename T> void _SafeRelease(T*& ptr)
{
	if (ptr) { ptr->Release(); ptr = nullptr; }
}
template<typename T> void _SafeDelete(T*& ptr)
{
	//if (ptr)					//조건 생략가.
	{ delete ptr;	ptr = nullptr; }
}
template<typename T> void _SafeDelArray(T*& ptr)
{
	//if (ptr)					//조건 생략가.
	{ delete[] ptr;	ptr = nullptr; }
}
#define SafeRelease		_SafeRelease
#define SafeDelete		_SafeDelete
#define SafeDelArray	_SafeDelArray
#endif

//싱글톤 렌더링 디바이스 객체 가져오기
RenderingDevice& RenderingDevice::GetRenderingDevice()
{
	static RenderingDevice RenderingDeviceSingleton;

	return RenderingDeviceSingleton;
}

// DisplayMode 설정
HRESULT RenderingDevice::SetDisplayMode(HWND hwnd)
{
	HRESULT hr = S_OK;

	//DXGI Display 모드 설정
	RECT rect;
	GetClientRect(hwnd, &rect);
	int BackBufferWidth = rect.right - rect.left;
	int BackBufferHeight = rect.bottom - rect.top;

	DisplayMode.Width = BackBufferWidth;
	DisplayMode.Height = BackBufferHeight;
	DisplayMode.RefreshRate = { 0, 1 };
	DisplayMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	return hr;
}

// Factory생성
HRESULT RenderingDevice::CreateFactory()
{
	HRESULT hr = S_OK;

	//DXGI Factory 생성
	UINT DXGIFactoryFlags = 0;
#if defined(_DEBUG) || !defined(NDEBUG)
	DXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	hr = CreateDXGIFactory2(DXGIFactoryFlags, IID_PPV_ARGS(Factory.GetAddressOf()));

	return hr;
}

// VRAM이 큰 Adapter를 찾아서 디바이스를 생성
HRESULT RenderingDevice::CreateHighSpecDevice()
{
	HRESULT hr = S_OK;

	//현재 사용중인 Adapter(GPU)를 찾고 D3D11을 지원하는 지 확인
	IDXGIAdapter1* Adapter = nullptr;
	IDXGIAdapter1* SelectedAdapter = nullptr;
	SIZE_T MaxDedicatedMemory = 0;

	for (UINT i = 0; Factory->EnumAdapters1(i, &Adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 Desc;
		Adapter->GetDesc1(&Desc);

		//CPU 기반 소프트웨어 렌더러라면 무시
		if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

		//Adapter가 DX11을 지원하는지 확인
		if (SUCCEEDED(D3D11CreateDevice(
			Adapter,
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

	assert(SelectedAdapter != nullptr && "No Compatible DX11 Adapter Found");

	//색상 Format 설정
	UINT DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG) || !defined(NDEBUG)
	DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = CreateDevice(SelectedAdapter, DeviceFlags);

	return hr;
}

//디바이스 생성
HRESULT RenderingDevice::CreateDevice(IDXGIAdapter1* Adapter, UINT DeviceFlags)
{
	HRESULT hr = S_OK;
	//D3D Device 생성
	hr = D3D11CreateDevice(
		Adapter,					//비디오 어댑터 포인터 (기본장치는 NULL)
		D3D_DRIVER_TYPE_UNKNOWN,	//어댑터를 작접 고른 경우 UNKNOWN
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

	return hr;
}

//SwapChain 생성
HRESULT RenderingDevice::CreateSwapChain(HWND hwnd)
{
	HRESULT hr = S_OK;
	//Swap Chain 생성
	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.Width = DisplayMode.Width;						//해상도 결정(백버퍼 크기)
	SwapChainDesc.Height = DisplayMode.Height;
	SwapChainDesc.Format = DisplayMode.Format;						//백버퍼 색상규격
	SwapChainDesc.Stereo = FALSE;									//스테레오 3D렌더링 옵션
	SwapChainDesc.SampleDesc = { 1, 0 };							//멀티 샘플링 설정 {Count, Quality}
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//용도 설정 : 렌더 타겟
	SwapChainDesc.BufferCount = 1;									//백버퍼 개수
	SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;					//출력 화면 크기가 모니터의 실제 해상도와 다를 때 어떻게 스케일링 할지 여부
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			//Present() 이후 백버퍼가 어떻게 처리되는지, 그리고 화면에 표시하는 방식이 무엇인지 정한다.
	SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;				//알파 처리 방식
	SwapChainDesc.Flags = 0;										//동작 옵션 플랙그

	//전체화면 모드 정보
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC FsDesc = {};
	ZeroMemory(&FsDesc, sizeof(FsDesc));
	FsDesc.RefreshRate.Numerator = IsVSync ? 60 : 0;				//버퍼 갱신율. (수직동기화 VSync 활성화시 표준갱신율 적용 : 60hz)
	FsDesc.RefreshRate.Denominator = IsVSync ? 1 : 0;
	FsDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	FsDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	FsDesc.Windowed = IsFullScreen ? FALSE : TRUE;

	hr = Factory->CreateSwapChainForHwnd(
		Device.Get(),
		hwnd,
		&SwapChainDesc,
		&FsDesc,
		nullptr,
		SwapChain.GetAddressOf());

	assert(SUCCEEDED(hr) && "스왑체인 생성 실패\n");

	return hr;
}

//BackBuffer를 가져온 후 RenderTargetView 생성
HRESULT RenderingDevice::SetBackBufferToRenderTargetView()
{
	HRESULT hr = S_OK;
	//렌더 타겟(백버퍼) 획득
	ID3D11Texture2D* BackBuffer = nullptr;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
	assert(SUCCEEDED(hr) && "백버퍼 가져오기 실패\n");

	//획득한 백버퍼에 렌더타겟 뷰 생성(렌더타겟 "형"으로 설정함)
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, RenderTargetView.GetAddressOf());
	assert(SUCCEEDED(hr) && "백버퍼 - 렌더타겟뷰 생성 실패\n");

	BackBuffer->Release();

	return hr;
}

//장치 출력병합기(Output Merger)에 렌더링 타겟 및 깊이-스탠실 버퍼 등록
HRESULT RenderingDevice::OMSetRenderTargets()
{
	HRESULT hr = S_OK;

	//장치 출력병합기(Output Merger)에 렌더링 타겟 및 깊이-스탠실 버퍼 등록
	DeviceContext->OMSetRenderTargets(
		1,									//렌더타겟 개수. (max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		RenderTargetView.GetAddressOf(),	//렌더타겟("백버퍼") 등록.
		nullptr
	);
	
	return hr;
}

//ViewPort 설정
HRESULT RenderingDevice::SetViewPort(float TopLeftX, float TopLeftY, float Width, float Height, float MinDepth, float MaxDepth)
{
	HRESULT hr = S_OK;

	// 뷰포트 설정
	D3D11_VIEWPORT ViewPort;
	ZeroMemory(&ViewPort, sizeof(ViewPort));
	ViewPort.TopLeftX = TopLeftX;
	ViewPort.TopLeftY = TopLeftY;
	ViewPort.Width = Width;
	ViewPort.Height = Height;
	ViewPort.MinDepth = MinDepth;
	ViewPort.MaxDepth = MaxDepth;
	DeviceContext->RSSetViewports(1, &ViewPort);

	return hr;
}

void RenderingDevice::ClearBackBuffer()
{
	DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), (float*)&BackGroundColor);
}

void RenderingDevice::RenderTargetPresent()
{
	//Present시 첫번째 인자 SyncInterval이 VSync 여부 ( 0 : 끔 , 1 : 켬(모니터 주사율에 맞춤) , 2 : 수직동기마다 출력)
	SwapChain->Present(IsVSync ? 1 : 0, 0);
}

//VertexBuffer 생성
//
// DX10 부터 버퍼 자원의 규격이 통합
// 버퍼 생성시 그 용도(VB, IB..)를 결정
// 
// param		VertexData		정점 데이터포인터
// param		Size			정점 데이터크기
// param		Stride			정점 데이터 하나의 크기
//
int RenderingDevice::CreateVertexBuffer(void* VertexData, UINT Size, UINT Stride)
{

	// 정점 버퍼 정보 구성
	D3D11_BUFFER_DESC Bd = {};
	ZeroMemory(&Bd, sizeof(Bd));
	Bd.Usage			= D3D11_USAGE_DEFAULT;		//버퍼 사용방식
	Bd.ByteWidth		= Size;						//버퍼 크기 sizeof(VTX_MESH) * 3
	Bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;	//버퍼 용도 : 정점 버퍼
	Bd.CPUAccessFlags	= 0;

	D3D11_SUBRESOURCE_DATA Rd;
	ZeroMemory(&Rd, sizeof(Rd));
	Rd.pSysMem = VertexData;						//버퍼에 저장될 데이터 : "정점들"

	//정점 버퍼 생성
	VertexBufferList.push_back(nullptr);
	VertexBufferSize++;

	HRESULT hr = Device->CreateBuffer(&Bd, &Rd, &VertexBufferList[VertexBufferSize]);
	assert(SUCCEEDED(hr) && "정점 버퍼 생성 실패");

	return VertexBufferSize;
}



//Font Create
SpriteFont* RenderingDevice::CreateSpriteFont()
{
	HRESULT hr = S_OK;

	//장치 목록 획득
	ID3D11DeviceContext* DC = nullptr;
	Device->GetImmediateContext(&DC);

	//DirectX Toolkit : Sprite Font 객체 생성
	//ASCII 0 ~ 255 + 특수문자'■' + Unicode 한글 완성형 총 11,440 글자, 크기:9	
	//exe 실행파일 기준의 경로
	const TCHAR* Filename = L"../../Engine/ForestPearlEngine/Assets/Font/굴림9k.sfont";

	SpriteFont* Font = nullptr;
	try
	{
		Font = new SpriteFont(Device.Get(), Filename);
		Font->SetLineSpacing(14.0f);					//폰트 9 기준, 줄간격 설정, '다중라인 출력시 흐려짐 방지용'
		Font->SetDefaultCharacter('_');					//출력 글자값 미검색시 대신 출력할 키값.
	}
	catch (std::exception& e)
	{
		TCHAR msg[1024] = L"";
		size_t converted = 0;

		errno_t result = mbstowcs_s(
			&converted,
			msg,
			_countof(msg),
			e.what(),
			_TRUNCATE
		);

		std::wcout << L"폰트 생성 실패 : " << msg << Filename << "\n";
		assert(FALSE && "폰트 생성 실패");
	}

	//사용 후, 장치 목록 해제
	SafeRelease(DC);

	return Font;
}

//Font Batch Create
SpriteBatch* RenderingDevice::CreateSpriteBatch()
{
	//장치 목록 획득
	ID3D11DeviceContext* DC = nullptr;
	Device->GetImmediateContext(&DC);

	SpriteBatch* Batch = new SpriteBatch(DC);

	//사용 후, 장치 목록 해제
	SafeRelease(DC);

	return Batch;
}

//Device 정보 얻기
void RenderingDevice::GetDeviceInfo()
{
	//장치 기능레벨 확인
	GetFeatureLevel();

	//GPU 정보 얻기
	GetAdapterInfo();
}

//DX 기능 레벨 구하기
void RenderingDevice::GetFeatureLevel()
{
	static const TCHAR* StrFeature[4][5] =
	{
		{ L"DX9",   L"DX9.1",  L"DX9.2", L"DX9.3", L"N/A"	},
		{ L"DX10",  L"DX10.1", L"N/A",   L"N/A",   L"N/A"	},
		{ L"DX11",  L"DX11.1", L"DX11.2",L"DX11.3",L"DX11.4"},
		{ L"DX12",  L"DX12.1"  L"DX12.2",L"N/A",   L"N/A"	}
	};

	UINT Feat = ActualLevel;
	UINT Ver = 0;
	UINT Sub = 0;

#define OFFSET 0x9;

	Ver = ((Feat & 0xf000) >> 12) - OFFSET;	//메인 버전 산출
	Sub = ((Feat & 0x0f00) >> 8);			//하위 버전 산출

	StrFeatureLevel = StrFeature[Ver][Sub];
}

HRESULT RenderingDevice::GetAdapterInfo()
{
	IDXGIAdapter1* Adapter;

	UINT i = 0;

	for (UINT i = 0; Factory->EnumAdapters1(i, &Adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DEVICEINFO Di;
		ZeroMemory(&Di, sizeof(Di));
		Di.Index = i;
		Adapter->GetDesc1(&Di.AdapterDescription);				//어뎁터 정보 획득.
		GetDXMonitorInfo(Adapter, Di);
		GetDXVRAMInfo(Adapter, Di);
		DevInfo.push_back(Di);

		SafeRelease(Adapter);
	}

	//정보 취득후, 접근한 인터페이스를 해제 (메모리 누수 방지
	SafeRelease(Adapter);

	return S_OK;
}

HRESULT RenderingDevice::GetDXMonitorInfo(IDXGIAdapter1* Adapter, DEVICEINFO& Di)
{
	IDXGIOutput* Output;
	DXGI_OUTPUT_DESC od;
	HRESULT hr = S_OK;

	for (UINT i = 0; Adapter->EnumOutputs(i, &Output) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		Output->GetDesc(&od); //정보 획득
		Di.OuputDesc.push_back(od);

		SafeRelease(Output);
	}

	return hr;
}

HRESULT RenderingDevice::GetDXVRAMInfo(IDXGIAdapter1* Adapter, DEVICEINFO& Di)
{
	IDXGIAdapter4* Adapter4 = nullptr;

	HRESULT hr = Adapter->QueryInterface(__uuidof(IDXGIAdapter4), reinterpret_cast<void**>(&Adapter4));

	ZeroMemory(&Di.VRAMInfo, sizeof(Di.VRAMInfo));

	hr = Adapter4->QueryVideoMemoryInfo(
		0,
		DXGI_MEMORY_SEGMENT_GROUP_LOCAL,
		&Di.VRAMInfo
	);

	return hr;
}

//장치 제거
HRESULT RenderingDevice::DeviceFinalize()
{
	//장치 상태 리셋 : 제거 전에 초기화 필수 (메모리 누수 방지)
	if (DeviceContext) DeviceContext->ClearState();

	RenderTargetView->Release();
	SwapChain->Release();
	DeviceContext->Release();
	Device->Release();

	return S_OK;
}