#include "RenderingDevice.h"
#include "../Utility/FPPathManager.h"
#include <assert.h>
#include <set>
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

//생성자
RenderingDevice::RenderingDevice()
{
	//상수 버퍼를 담을 공간의 크기를 MAX_BufferSize로 설정
	VertexShaderConstBuffer.resize(MAX_BufferSize);
	PixelShaderConstBuffer.resize(MAX_BufferSize);
}

DXGI_FORMAT RenderingDevice::FPRenderTargetFormatToDXGI_FORMAT(FPRenderTargetFormat& RenderTargetFormat)
{
	static const std::unordered_map<FPRenderTargetFormat, DXGI_FORMAT> FormatMap =
	{
		{ FPRenderTargetFormat::R8,         DXGI_FORMAT_R8_UNORM },
		{ FPRenderTargetFormat::RG8,        DXGI_FORMAT_R8G8_UNORM },
		{ FPRenderTargetFormat::RGBA8,      DXGI_FORMAT_R8G8B8A8_UNORM },
		{ FPRenderTargetFormat::RGBA8_SRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB },
		{ FPRenderTargetFormat::R16F,       DXGI_FORMAT_R16_FLOAT },
		{ FPRenderTargetFormat::RGBA16F,    DXGI_FORMAT_R16G16B16A16_FLOAT },
		{ FPRenderTargetFormat::R32F,       DXGI_FORMAT_R32_FLOAT },
		{ FPRenderTargetFormat::RGBA32F,    DXGI_FORMAT_R32G32B32A32_FLOAT },
		{ FPRenderTargetFormat::RGBA10A2,   DXGI_FORMAT_R10G10B10A2_UNORM },
	};

	auto it = FormatMap.find(RenderTargetFormat);

	if (it != FormatMap.end())
	{
		return it->second;
	}

	//기본값으로 설정된 RGBA8
	return DXGI_FORMAT_R8G8B8A8_UNORM;
}

DXGI_FORMAT RenderingDevice::FPDepthStencilFormatToDXGI_FORMAT(FPDepthStencilFormat& DepthStencilFormat)
{
	static const std::unordered_map<FPDepthStencilFormat, DXGI_FORMAT> FormatMap =
	{
		{ FPDepthStencilFormat::D16,            DXGI_FORMAT_D16_UNORM },
		{ FPDepthStencilFormat::D24_S8_UINT,     DXGI_FORMAT_D24_UNORM_S8_UINT },
		{ FPDepthStencilFormat::D32F,            DXGI_FORMAT_D32_FLOAT },
		{ FPDepthStencilFormat::D32F_S8X24_UINT, DXGI_FORMAT_D32_FLOAT_S8X24_UINT },
	};

	auto it = FormatMap.find(DepthStencilFormat);

	if (it != FormatMap.end())
	{
		return it->second;
	}

	//기본값으로 설정된 D24_S8_UINT
	return DXGI_FORMAT_D24_UNORM_S8_UINT;
}

DXGI_FORMAT RenderingDevice::DSVFormatToTextureFormat(DXGI_FORMAT& DSVFormat)
{
	static const std::unordered_map<DXGI_FORMAT, DXGI_FORMAT> FormatMap =
	{
		{ DXGI_FORMAT_D16_UNORM,            DXGI_FORMAT_R16_TYPELESS },
		{ DXGI_FORMAT_D24_UNORM_S8_UINT,    DXGI_FORMAT_R24G8_TYPELESS },
		{ DXGI_FORMAT_D32_FLOAT,            DXGI_FORMAT_R32_TYPELESS },
		{ DXGI_FORMAT_D32_FLOAT_S8X24_UINT, DXGI_FORMAT_R32G8X24_TYPELESS },
	};

	auto it = FormatMap.find(DSVFormat);

	if (it != FormatMap.end())
	{
		return it->second;
	}

	return DSVFormat;
}

DXGI_FORMAT RenderingDevice::DSVFormatToSRVFormat(DXGI_FORMAT& DSVFormat)
{
	static const std::unordered_map<DXGI_FORMAT, DXGI_FORMAT> FormatMap =
	{
		{ DXGI_FORMAT_D16_UNORM,            DXGI_FORMAT_R16_UNORM },
		{ DXGI_FORMAT_D24_UNORM_S8_UINT,    DXGI_FORMAT_R24_UNORM_X8_TYPELESS },
		{ DXGI_FORMAT_D32_FLOAT,            DXGI_FORMAT_R32_FLOAT },
		{ DXGI_FORMAT_D32_FLOAT_S8X24_UINT, DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS },
	};

	auto it = FormatMap.find(DSVFormat);

	if (it != FormatMap.end())
	{
		return it->second;
	}

	return DSVFormat;
}

D3D11_STENCIL_OP RenderingDevice::FPStencilOPToD3DStencilOP(FPStencilOP& StencilOP)
{
	static const std::unordered_map<FPStencilOP, D3D11_STENCIL_OP> FormatMap =
	{
		{ FPStencilOP::KEEP,     D3D11_STENCIL_OP_KEEP },
		{ FPStencilOP::ZERO,     D3D11_STENCIL_OP_ZERO },
		{ FPStencilOP::REPLACE,  D3D11_STENCIL_OP_REPLACE },
		{ FPStencilOP::INCR_SAT, D3D11_STENCIL_OP_INCR_SAT },
		{ FPStencilOP::DECR_SAT, D3D11_STENCIL_OP_DECR_SAT },
		{ FPStencilOP::INVERT,   D3D11_STENCIL_OP_INVERT },
		{ FPStencilOP::INCR,     D3D11_STENCIL_OP_INCR },
		{ FPStencilOP::DECR,     D3D11_STENCIL_OP_DECR },
	};

	auto it = FormatMap.find(StencilOP);

	if (it != FormatMap.end())
	{
		return it->second;
	}

	//Default : KEEP
	return D3D11_STENCIL_OP_KEEP;
}

D3D11_COMPARISON_FUNC RenderingDevice::FPCompraisionFuncToD3DComparisonFunc(FPCompraisonFunc& CompraisonFunc)
{
	static const std::unordered_map<FPCompraisonFunc, D3D11_COMPARISON_FUNC> FormatMap =
	{
		{ FPCompraisonFunc::NERVER,        D3D11_COMPARISON_NEVER },
		{ FPCompraisonFunc::LESS,          D3D11_COMPARISON_LESS },
		{ FPCompraisonFunc::EQUAL,         D3D11_COMPARISON_EQUAL },
		{ FPCompraisonFunc::LESS_EQUAL,    D3D11_COMPARISON_LESS_EQUAL },
		{ FPCompraisonFunc::GREATER,       D3D11_COMPARISON_GREATER },
		{ FPCompraisonFunc::NOT_EQUAL,     D3D11_COMPARISON_NOT_EQUAL },
		{ FPCompraisonFunc::GREATER_EQUAL, D3D11_COMPARISON_GREATER_EQUAL },
		{ FPCompraisonFunc::ALWAYS,        D3D11_COMPARISON_ALWAYS },
	};

	auto it = FormatMap.find(CompraisonFunc);

	if (it != FormatMap.end())
	{
		return it->second;
	}

	//Default : ALWAYS
	return D3D11_COMPARISON_ALWAYS;
}

D3D11_DEPTH_WRITE_MASK RenderingDevice::FPDepthWriteMaskToD3DDepthWriteMask(FPDepthWriteMask& DepthWriteMask)
{
	static const std::unordered_map<FPDepthWriteMask, D3D11_DEPTH_WRITE_MASK> FormatMap =
	{
		{ FPDepthWriteMask::ZERO, D3D11_DEPTH_WRITE_MASK_ZERO },
		{ FPDepthWriteMask::ALL,  D3D11_DEPTH_WRITE_MASK_ALL },
	};

	auto it = FormatMap.find(DepthWriteMask);

	if (it != FormatMap.end())
	{
		return it->second;
	}

	//Default : ALL
	return D3D11_DEPTH_WRITE_MASK_ALL;
}

//싱글톤 렌더링 디바이스 객체 가져오기
RenderingDevice& RenderingDevice::GetRenderingDevice()
{
	static RenderingDevice RenderingDeviceSingleton;

	return RenderingDeviceSingleton;
}

HRESULT RenderingDevice::SetMSAA(int MSAA) { this->dwAA = MSAA; return S_OK; }
HRESULT RenderingDevice::SetAF(int AF) { this->dwAF = AF; return S_OK; }

void RenderingDevice::SetVSync(bool VSync) { this->IsVSync = VSync; }
void RenderingDevice::SetHz(int Hz) { this->RefreshRate = Hz; }
void RenderingDevice::SetSwapChainRect(int Width, int Height) { this->SwapChainWidth = Width; this->SwapChainHeight = Height; }


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

	int BackBufferWidth = (SwapChainWidth > 0) ? SwapChainWidth : DisplayMode.Width;
	int BackBufferHeight = (SwapChainHeight > 0) ? SwapChainHeight : DisplayMode.Height;
	SwapChainDesc.Width = BackBufferWidth;						//해상도 결정(백버퍼 크기)
	SwapChainDesc.Height = BackBufferHeight;
	SwapChainDesc.Format = DisplayMode.Format;						//백버퍼 색상규격
	SwapChainDesc.Stereo = FALSE;									//스테레오 3D렌더링 옵션
	SwapChainDesc.SampleDesc = { dwAA, 0 };							//멀티 샘플링 설정 {Count, Quality} AA 설정 "MSAA"
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//용도 설정 : 렌더 타겟
	SwapChainDesc.BufferCount = 1;									//백버퍼 개수
	SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;					//출력 화면 크기가 모니터의 실제 해상도와 다를 때 어떻게 스케일링 할지 여부
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			//Present() 이후 백버퍼가 어떻게 처리되는지, 그리고 화면에 표시하는 방식이 무엇인지 정한다.
	SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;				//알파 처리 방식
	SwapChainDesc.Flags = 0;										//동작 옵션 플랙그

	//전체화면 모드 정보
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC FsDesc = {};
	ZeroMemory(&FsDesc, sizeof(FsDesc));
	FsDesc.RefreshRate.Numerator = IsVSync ? RefreshRate : 0;				//버퍼 갱신율. (수직동기화 VSync 활성화시 표준갱신율 적용 : 60hz)
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

HRESULT RenderingDevice::CreateRTTex(UINT SizeX, UINT SizeY, UINT MSAA, bool MipMap, FPRenderTargetFormat& RenderTargetFormat, void** OutRTTex, void** OutResolveTexture)
{
	HRESULT hr = S_OK;
	//텍스처 정보 구성

	//RenderTarget용 Texture
	D3D11_TEXTURE2D_DESC TextureDesc = {};
	TextureDesc.Width = SizeX;
	TextureDesc.Height = SizeY;
	TextureDesc.MipLevels = 1;					//MSAA를 사용하는 경우 MipLevels를 1로 설정
	TextureDesc.ArraySize = 1;					//추후 다른 Type의 RT Tex를 만들때 변경 필요
	TextureDesc.Format = FPRenderTargetFormatToDXGI_FORMAT(RenderTargetFormat);
	TextureDesc.SampleDesc.Count = MSAA;		//기본 설정은 1이나 MSAA가 사용될 수 있으므로 SRV Texture는 Resolve함
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;	//용도 RT
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	ID3D11Texture2D* RTTex = nullptr;
	hr = Device->CreateTexture2D(&TextureDesc, NULL, &RTTex);
	if (FAILED(hr)) { std::cout << "RT Tex 생성 실패" << "\n"; }

	//외부로 Return
	*OutRTTex = RTTex;

	//SRV용 Texture
	//RenderTarget용 Texture와 구성은 같으나 일부 설정이 다름
	TextureDesc.SampleDesc.Count = 1;		//Texture2D Sample 및 MipMap 생성을 위해서 MSAA 설정을 끈 Texture 생성
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//용도 Shader Resource
	//자동 MipMap 설정을 킨 경우
	if (MipMap)
	{
		TextureDesc.MipLevels = 0;	//전체 MipMap 생성
		TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	//용도 RT + SRV, MipMap 자동 생성을 위해
		TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}


	ID3D11Texture2D* SRVTex = nullptr;
	hr = Device->CreateTexture2D(&TextureDesc, NULL, &SRVTex);
	if (FAILED(hr)) { std::cout << "SRV Tex 생성 실패" << "\n"; return hr; }

	//외부로 Return
	*OutResolveTexture = SRVTex;

	return hr;
}

HRESULT RenderingDevice::CreateRTView(FPRenderTargetFormat& RenderTargetFormat, UINT MSAA, void* InRTTex, void** OutRTView)
{
	D3D11_RENDER_TARGET_VIEW_DESC  RTVDesc = {};

	RTVDesc.Format = FPRenderTargetFormatToDXGI_FORMAT(RenderTargetFormat);		//Texture 와 동일 Format  유지
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;						//2D RT
	RTVDesc.Texture2D.MipSlice = 0;												//2D RT 용 추가 설정 : 밉맵 분할용 밉맵레벨 인덱스

	//MSAA 설정 사용
	if (MSAA > 1)
	{
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		RTVDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
	}

	//렌더타겟 뷰 생성
	ID3D11RenderTargetView* RTView = nullptr;
	ID3D11Texture2D* InTex = static_cast<ID3D11Texture2D*>(InRTTex);
	HRESULT hr = Device->CreateRenderTargetView(InTex, &RTVDesc, &RTView);
	if (FAILED(hr)) { std::cout << "RTV 생성 실패" << "\n"; return hr; }

	//외부 리턴
	*OutRTView = RTView;

	return hr;
}

HRESULT RenderingDevice::CreateRTSRView(FPRenderTargetFormat& RenderTargetFormat, void* InRTRelsolveTex, void** OutSRVView)
{
	
	D3D11_SHADER_RESOURCE_VIEW_DESC  SRVVDesc = {};

	SRVVDesc.Format = FPRenderTargetFormatToDXGI_FORMAT(RenderTargetFormat);		//Texture 와 동일 Format  유지
	SRVVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;							//2D RT
	SRVVDesc.Texture2D.MipLevels = -1;												//2D SRV용 추가 설정 : 밉맵 분할용 밉맵레벨 인덱스
	SRVVDesc.Texture2D.MostDetailedMip = 0;
	//MSAA 설정은 필요시 진행
	

	//렌더타겟 뷰 생성
	ID3D11ShaderResourceView* SRVView = nullptr;
	ID3D11Texture2D* InTex = static_cast<ID3D11Texture2D*>(InRTRelsolveTex);
	HRESULT hr = Device->CreateShaderResourceView(InTex, &SRVVDesc, &SRVView);
	if (FAILED(hr)) { std::cout << "RTSRV 생성 실패" << "\n"; return hr; }

	//외부 리턴
	*OutSRVView = SRVView;

	return hr;
}

HRESULT RenderingDevice::CreateDepthStencilTexture(UINT SizeX, UINT SizeY, UINT MSAA, bool ShaderResource, FPDepthStencilFormat& DepthStencilFormat, void** OutDSTex)
{
	HRESULT hr = S_OK;

	//깊이 - 스텐실 버퍼용 정보 구성
	D3D11_TEXTURE2D_DESC TextureDesc;
	TextureDesc.Width = SizeX;
	TextureDesc.Height = SizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = FPDepthStencilFormatToDXGI_FORMAT(DepthStencilFormat);						//깊이 버퍼 Format
	TextureDesc.SampleDesc = { MSAA, 0 };				//MSAA 설정
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//깊이-스텐실 버퍼용으로 설정
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (ShaderResource)
	{
		TextureDesc.Format = DSVFormatToTextureFormat(TextureDesc.Format);			//ShaderResource로도 사용된다면 해당 해당 타입으로 변환
		TextureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;						//ShaderResource Binding 추가
	}

	ID3D11Texture2D* Texture;
	hr = Device->CreateTexture2D(&TextureDesc, NULL, &Texture);
	if (FAILED(hr)) { std::cout << "DepthStencil Texture Create Fail!" << "\n"; return hr; }
	*OutDSTex = Texture;

	return hr;
}

HRESULT RenderingDevice::CreateDepthStencilView(FPDepthStencilFormat& DepthStencilFormat, UINT MSAA, void* InDSTex, void** OutDSView)
{
	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
	DSVDesc.Format = FPDepthStencilFormatToDXGI_FORMAT(DepthStencilFormat);
	DSVDesc.Flags = 0;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;

	if (MSAA > 1)
	{
		DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		DSVDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
	}
	
	ID3D11DepthStencilView* DSV;
	ID3D11Texture2D* DSTex = static_cast<ID3D11Texture2D*>(InDSTex);

	hr = Device->CreateDepthStencilView(DSTex, &DSVDesc, &DSV);
	if (FAILED(hr)) { std::cout << "DSV Create Fail!" << "\n"; return hr; }
	
	*OutDSView = DSV;
	return hr;
}

HRESULT RenderingDevice::CreateDepthStencilSRView(FPDepthStencilFormat& DepthStencilFormat, bool ShaderResource, UINT MSAA, void* InDSTex, void** OutDSSRView)
{
	HRESULT hr = S_OK;
	if (!ShaderResource) return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	DXGI_FORMAT DSVFormat = FPDepthStencilFormatToDXGI_FORMAT(DepthStencilFormat);
	SRVDesc.Format = DSVFormatToSRVFormat(DSVFormat);
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;
	if (MSAA > 1)
	{
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		SRVDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
	}

	ID3D11ShaderResourceView* DSSRV;
	ID3D11Texture2D* DSTex = static_cast<ID3D11Texture2D*>(InDSTex);

	hr = Device->CreateShaderResourceView(DSTex, &SRVDesc, &DSSRV);
	if (FAILED(hr)) { std::cout << "DSSRV Create Fail!" << "\n"; return hr; }

	*OutDSSRView = DSSRV;
	return hr;
}

HRESULT RenderingDevice::CreateDepthStencilState(FPDepthStencilStateInfo& DepthStencilInfo, void** OutDSState)
{
	HRESULT hr = S_OK;
	D3D11_DEPTH_STENCIL_DESC Desc = {};

	//Depth 설정
	Desc.DepthEnable = DepthStencilInfo.DepthEnable ? TRUE : FALSE;
	Desc.DepthWriteMask = FPDepthWriteMaskToD3DDepthWriteMask(DepthStencilInfo.DepthWriteMask);
	Desc.DepthFunc = FPCompraisionFuncToD3DComparisonFunc(DepthStencilInfo.DepthFunc);

	// Stencil 설정
	Desc.StencilEnable = DepthStencilInfo.StencilEnable ? TRUE : FALSE;
	Desc.StencilReadMask = static_cast<UINT8>(DepthStencilInfo.StencilReadMask);
	Desc.StencilWriteMask = static_cast<UINT8>(DepthStencilInfo.StencilWriteMask);

	// 앞면 Stencil 연산
	Desc.FrontFace.StencilFailOp = FPStencilOPToD3DStencilOP(DepthStencilInfo.FrontFaceStencilFailOp);
	Desc.FrontFace.StencilDepthFailOp = FPStencilOPToD3DStencilOP(DepthStencilInfo.FrontStencilDepthFailOp);
	Desc.FrontFace.StencilPassOp = FPStencilOPToD3DStencilOP(DepthStencilInfo.FrontStencilPassOp);
	Desc.FrontFace.StencilFunc = FPCompraisionFuncToD3DComparisonFunc(DepthStencilInfo.FrontStencilFunc);

	// 뒷면 Stencil 연산
	Desc.BackFace.StencilFailOp = FPStencilOPToD3DStencilOP(DepthStencilInfo.BackFaceStencilFailOp);
	Desc.BackFace.StencilDepthFailOp = FPStencilOPToD3DStencilOP(DepthStencilInfo.BackStencilDepthFailOp);
	Desc.BackFace.StencilPassOp = FPStencilOPToD3DStencilOP(DepthStencilInfo.BackStencilPassOp);
	Desc.BackFace.StencilFunc = FPCompraisionFuncToD3DComparisonFunc(DepthStencilInfo.BackStencilFunc);

	ID3D11DepthStencilState* State;
	hr = Device->CreateDepthStencilState(&Desc, &State);
	if (FAILED(hr)) { std::cout << "DepthStencil State Create Fail!" << "\n"; return hr; }

	*OutDSState = State;
	return hr;
}

//BackBuffer를 가져온 후 RenderTargetView 생성
HRESULT RenderingDevice::CreateBackBufferRenderTarget()
{
	HRESULT hr = S_OK;
	//렌더 타겟(백버퍼) 획득
	ID3D11Texture2D* BackBuffer = nullptr;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
	assert(SUCCEEDED(hr) && "백버퍼 가져오기 실패\n");

	//획득한 백버퍼에 렌더타겟 뷰 생성(렌더타겟 "형"으로 설정함)
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, BackBufferRenderTargetView.GetAddressOf());
	assert(SUCCEEDED(hr) && "백버퍼 - 렌더타겟뷰 생성 실패\n");

	BackBuffer->Release();

	return hr;
}

//BackBufferRenderTarget Setting
HRESULT RenderingDevice::OMSetBackBufferRenderTargets(UINT RenderTargetNum, void* InDepthStencilView)
{
	HRESULT hr = S_OK;

	ID3D11DepthStencilView* DepthStencilView = static_cast<ID3D11DepthStencilView*>(InDepthStencilView);
	DeviceContext->OMSetRenderTargets(RenderTargetNum, BackBufferRenderTargetView.GetAddressOf(), DepthStencilView);

	return hr;
}

HRESULT RenderingDevice::OMSetRenderTargets(UINT RenderTargetNum, void* InRenderTargetView, void* InDepthStencilView)
{
	HRESULT hr = S_OK;

	ID3D11RenderTargetView* RenderTargetView = static_cast<ID3D11RenderTargetView*>(InRenderTargetView);
	ID3D11DepthStencilView* DepthStencilView = static_cast<ID3D11DepthStencilView*>(InDepthStencilView);

	DeviceContext->OMSetRenderTargets(RenderTargetNum, &RenderTargetView, DepthStencilView);

	return hr;
}

HRESULT RenderingDevice::OMResetRenderTargets()
{
	HRESULT hr = S_OK;

	DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

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

HRESULT RenderingDevice::SetBackGroundColor(float R, float G, float B, float A)
{
	BackGroundColor.x = R;
	BackGroundColor.y = G;
	BackGroundColor.z = B;
	BackGroundColor.w = A;

	return S_OK;
}

void RenderingDevice::ClearBackBuffer(void* InDepthStencilView)
{
	ID3D11DepthStencilView* DepthStencilView = static_cast<ID3D11DepthStencilView*>(InDepthStencilView);

	DeviceContext->ClearRenderTargetView(BackBufferRenderTargetView.Get(), (float*)&BackGroundColor);
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderingDevice::ClearRenderTargetView(void* InRenderTargetView, float* ClearColor)
{
	ID3D11RenderTargetView* RenderTargetView = static_cast<ID3D11RenderTargetView*>(InRenderTargetView);
	DeviceContext->ClearRenderTargetView(RenderTargetView, ClearColor);
}

void RenderingDevice::ClearDepthStencilView(void* InDepthStencilView)
{
	ID3D11DepthStencilView* DepthStencilView = static_cast<ID3D11DepthStencilView*>(InDepthStencilView);
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderingDevice::RenderTargetPresent()
{
	//Present시 첫번째 인자 SyncInterval이 VSync 여부 ( 0 : 끔 , 1 : 켬(모니터 주사율에 맞춤) , 2 : 수직동기마다 출력)
	SwapChain->Present(IsVSync ? 1 : 0, 0);
}

//
// [DX 버퍼의 종류]
// 
// D3D11_BUFFER_DESC.Usage
// 
//	1. 기본 버퍼 : Default , GPU의 읽기/쓰기. 통상정인 버퍼 사용. 내용을 갱신할때는 UpdateSubResource 사용 (VRAM)
//  2. 동적 버퍼 : Dynamic , CPU (쓰기) GPU (읽기), 빈번한 내용 변화시, 내용을 갱신할때는 Map / Unmap 사용 (RAM의 공유 메모리)
//  3. 기타 :	Immutable	: GPU(읽기 전용), CPU 접근 불가, 내용 변경 불가 (VRAM)
//				Staging		: GPU에서 CPU로 복사 가능.	 (RAM의 공유 메모리 중에서도 GPU Write에 최적화된 메모리)
//



//VertexBuffer 생성
//
// DX10 부터 버퍼 자원의 규격이 통합
// 버퍼 생성시 그 용도(VB, IB..)를 결정
// 
// param		VertexData		정점 데이터포인터
// param		Size			정점 데이터크기
// param		Stride			정점 데이터 하나의 크기
//
void* RenderingDevice::CreateVertexBuffer(void* VertexData, UINT Size, UINT Stride)
{
	// 정점 버퍼 정보 구성
	D3D11_BUFFER_DESC Bd = {};
	ZeroMemory(&Bd, sizeof(Bd));
	Bd.Usage			= D3D11_USAGE_DEFAULT;		//버퍼 사용방식
	Bd.ByteWidth		= Size * Stride;						//버퍼 크기
	Bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;	//버퍼 용도 : 정점 버퍼
	Bd.CPUAccessFlags	= 0;

	D3D11_SUBRESOURCE_DATA Rd;
	ZeroMemory(&Rd, sizeof(Rd));
	Rd.pSysMem = VertexData;						//버퍼에 저장될 데이터 : "정점들"

	//정점 버퍼 생성
	ID3D11Buffer* VertexBuffer = nullptr;

	HRESULT hr = Device->CreateBuffer(&Bd, &Rd, &VertexBuffer);
	assert(SUCCEEDED(hr) && "정점 버퍼 생성 실패");

	return VertexBuffer;
}

void* RenderingDevice::CreateIndexBuffer(void* IndexData, UINT Size, UINT Stride)
{
	// 정점 버퍼 정보 구성
	D3D11_BUFFER_DESC Bd = {};
	ZeroMemory(&Bd, sizeof(Bd));
	Bd.Usage = D3D11_USAGE_DEFAULT;		//버퍼 사용방식
	Bd.ByteWidth = Size * Stride;			//버퍼 크기
	Bd.BindFlags = D3D11_BIND_INDEX_BUFFER;	//버퍼 용도 : 색인 버퍼
	Bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA Rd;
	ZeroMemory(&Rd, sizeof(Rd));
	Rd.pSysMem = IndexData;						//버퍼에 저장될 데이터 : "정점들"

	//정점 버퍼 생성
	ID3D11Buffer* IndexBuffer = nullptr;

	HRESULT hr = Device->CreateBuffer(&Bd, &Rd, &IndexBuffer);
	assert(SUCCEEDED(hr) && "정점 버퍼 생성 실패");

	return IndexBuffer;
}

// 상수 버퍼 생성
//
// 14개 등록 가능. 다른 셰이더와 혼용 가능.
// 셰이더 소스에 임의 지정 가능. register(b#)으로 지정, 약어 b = 상수버퍼
//

HRESULT RenderingDevice::CreateVertexShaderConstBuffer(UINT Size)
{
	HRESULT hr = S_OK;
	for (ID3D11Buffer*& ConstBuffer : VertexShaderConstBuffer)
	{
		hr = CreateConstBuffer(Size, &ConstBuffer);
	}

	return hr;
}

HRESULT RenderingDevice::CreatePixelShaderConstBuffer(UINT Size)
{
	HRESULT hr = S_OK;
	for (ID3D11Buffer*& ConstBuffer : PixelShaderConstBuffer)
	{
		hr = CreateConstBuffer(Size, &ConstBuffer);
	}

	return hr;
}

int RenderingDevice::CreateConstBuffer(UINT Size, ID3D11Buffer** ReturnConstBuffer)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC BufferDesc = {};
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = Size;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//상수 버퍼 생성
	ID3D11Buffer* pConstBuffer = nullptr;
	hr = Device->CreateBuffer(&BufferDesc, nullptr, &pConstBuffer);
	assert(SUCCEEDED(hr) && "상수 버퍼 생성 실패\n");

	*ReturnConstBuffer = pConstBuffer;



	return 0;
}

HRESULT RenderingDevice::VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers)
{
	HRESULT hr = S_OK;
	DeviceContext->VSSetConstantBuffers(StartSlot, NumBuffers, &VertexShaderConstBuffer[StartSlot]);
	return hr;
}

HRESULT RenderingDevice::PSSetConstantBuffers(UINT StartSlot, UINT NumBuffers)
{
	HRESULT hr = S_OK;
	DeviceContext->PSSetConstantBuffers(StartSlot, NumBuffers, &PixelShaderConstBuffer[StartSlot]);
	return hr;
}

HRESULT RenderingDevice::UpdateVertexShaderSubresource(UINT Slot, const uint8_t* pSrcData, UINT Offset, UINT Size)
{
	assert(pSrcData != nullptr);
	assert(VertexShaderConstBuffer[Slot] != nullptr);

	HRESULT hr = S_OK;
	//memcpy 형식으로 변경 (고정 크기 상수 버퍼에 대해서 상수 버퍼보다 크기가 작은 가변 크기 사용자 상수 버퍼를 대응하기 위해)
	D3D11_MAPPED_SUBRESOURCE Mapped{};
	
	hr = DeviceContext->Map(VertexShaderConstBuffer[Slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);

	if (FAILED(hr))
	{
		return hr;
	}

	assert(Mapped.pData != nullptr);

	memcpy( Mapped.pData, (pSrcData+Offset), Size);

	DeviceContext->Unmap(VertexShaderConstBuffer[Slot], 0);

	//DeviceContext->UpdateSubresource(VertexShaderConstBuffer[Slot], DstSubresource, nullptr, pSrcData, SrcRowPitch, SrcDepthPitch);

	return hr;
}

HRESULT RenderingDevice::UpdatePixelShaderSubresource(UINT Slot, const uint8_t* pSrcData, UINT Offset, UINT Size)
{
	assert(pSrcData != nullptr);
	assert(PixelShaderConstBuffer[Slot] != nullptr);

	HRESULT hr = S_OK;
	//memcpy 형식으로 변경 (고정 크기 상수 버퍼에 대해서 상수 버퍼보다 크기가 작은 가변 크기 사용자 상수 버퍼를 대응하기 위해)
	D3D11_MAPPED_SUBRESOURCE Mapped{};

	hr = DeviceContext->Map(PixelShaderConstBuffer[Slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);

	if (FAILED(hr))
	{
		return hr;
	}

	assert(Mapped.pData != nullptr);

	memcpy(Mapped.pData, (pSrcData + Offset), Size);

	DeviceContext->Unmap(PixelShaderConstBuffer[Slot], 0);

	//DeviceContext->UpdateSubresource(PixelShaderConstBuffer[Slot], DstSubresource, nullptr, pSrcData, SrcRowPitch, SrcDepthPitch);
	return hr;
}


//추후에 Shader Factory에서 처리할지 고민 (책임 분리)
HRESULT RenderingDevice::ReflectionShader(void* VScode, void* PScode, const std::set<std::pair<FPShaderType, std::string> >& ManagedCBs, FPShaderReflectionData* OutReflectionData, FPConstantBufferRenderData* OutVS, FPConstantBufferRenderData* OutPS)
{
	HRESULT hr = S_OK;
	std::set<std::pair<FPShaderType, std::string> > FoundCBs;
	FPShaderReflectionData NextLayout;
	FPConstantBufferRenderData NextVS;
	FPConstantBufferRenderData NextPS;

	//Pass 0 : VS, Pass 1 : PS
	for (unsigned int Pass = 0; Pass < 2; ++Pass)
	{
		const FPShaderType Stage = Pass == 0 ? FPShaderType::VS : FPShaderType::PS;
		FPConstantBufferRenderData& Destination = Pass == 0 ? NextVS : NextPS;
		ID3DBlob* ByteCode = Pass == 0 ? static_cast<ID3DBlob*>(VScode) : static_cast<ID3DBlob*>(PScode);

		//셰이더 리플렉션 정보를 받아옴
		ID3D11ShaderReflection* Reflection = nullptr;
		hr = D3DReflect(ByteCode->GetBufferPointer(), ByteCode->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&Reflection));
		if (FAILED(hr) || Reflection == nullptr) { std::cout << "Shader Reflection Fail!" << "\n"; return E_FAIL; }

		D3D11_SHADER_DESC ShaderDesc = {};
		hr = Reflection->GetDesc(&ShaderDesc);
		if (FAILED(hr)) { std::cout << "Shader 메타데이터 조회 실패!" << "\n"; return E_FAIL; }

		for (UINT Index = 0; Index < ShaderDesc.BoundResources; ++Index)
		{
			D3D11_SHADER_INPUT_BIND_DESC Binding = {};
			hr = Reflection->GetResourceBindingDesc(Index, &Binding);
			if (FAILED(hr)) continue;

			const std::string Name = Binding.Name;

			//바인딩 타입이 상수버퍼인경우
			if (Binding.Type == D3D_SIT_CBUFFER)
			{
				// JSON에 없는 CB들 (ex: Light/Object/Viewport )등은 내부 정보도 조회하지 않음
				if (ManagedCBs.count(std::make_pair(Stage, Name)) == 0) continue;

				//상수버퍼 정보를 가져옴
				ID3D11ShaderReflectionConstantBuffer* CB = Reflection->GetConstantBufferByName(Name.c_str());
				D3D11_SHADER_BUFFER_DESC CBDesc = {};
				hr = CB->GetDesc(&CBDesc);
				if (CB == nullptr || FAILED(hr) || CBDesc.Size == 0 || CBDesc.Size % 16 != 0 || CBDesc.Size > 9232)
				{
					std::cout << "상수 버퍼 메타데이터 또는 크기가 유효하지 않음 : " + Name << "\n";
					return E_FAIL;
				}

				// CPU 버퍼는 VS, PS별 연속 메모리로 확보하고 개별 CB의 위치를 기록
				const size_t CBStart = Destination.Buffer.size();
				Destination.Buffer.resize(CBStart + CBDesc.Size, 0);
				Destination.ConstantBuffers.push_back({ Binding.BindPoint, CBStart, CBDesc.Size });
				FoundCBs.insert(std::make_pair(Stage, Name));

				const size_t OldCount = NextLayout.Parameters.size();
				for (UINT VariableIndex = 0; VariableIndex < CBDesc.Variables; ++VariableIndex)
				{
					//각 버퍼의 데이터를 가져옴
					ID3D11ShaderReflectionVariable* Variable = CB->GetVariableByIndex(VariableIndex);
					D3D11_SHADER_VARIABLE_DESC VarDesc = {};
					hr = Variable->GetDesc(&VarDesc);
					if (FAILED(hr) || VarDesc.Name == nullptr) continue;
					if (VarDesc.StartOffset > CBDesc.Size || VarDesc.Size > CBDesc.Size - VarDesc.StartOffset)
					{
						std::cout << "상수 버퍼 변수 범위를 벗어남: " + Name << "\n";
						return E_FAIL;
					}

					//AppendMaterialParameter (각 버퍼의 데이터 등록)
					hr = AppendMaterialParameter(Variable->GetType(), VarDesc.Name, VarDesc.StartOffset, CBDesc.Size, CBStart, Stage, Name, NextLayout);
					if (FAILED(hr))
					{
						std::cout << "구조체 분석 실패: " + Name << "\n";
						return E_FAIL;
					}
				}

				if (NextLayout.Parameters.size() == OldCount)
				{
					std::cout << "MaterialConstBuffer에서 지원하는 Parameter가 없음" + Name << "\n";
					return E_FAIL;
				}
			}
			else if (Stage == FPShaderType::PS && (Binding.Type == D3D_SIT_TEXTURE || Binding.Type == D3D_SIT_SAMPLER)) //PS 이면서, Texture, Sampler 처리
			{
				// 배열은 BindPoint에서 시작하여 BindCount개의 연속 슬롯을 점유
				const unsigned int Limit = Binding.Type == D3D_SIT_TEXTURE ? 128u : 16u;
				if (Binding.BindCount == 0 || Binding.BindPoint >= Limit || Binding.BindCount > Limit - Binding.BindPoint)
				{
					std::cout << "셰이더 리소스의 슬롯 범위가 유효하지 않음: " + Name << "\n";
					return E_FAIL;
				}

				if (Binding.Type == D3D_SIT_SAMPLER)
				{
					// 샘플러 배열도 원소마다 독립된 상태와 실제 슬롯을 보관
					for (unsigned int ArrayIndex = 0; ArrayIndex < Binding.BindCount; ++ArrayIndex)
					{
						FPReflectionSamplerInfo Entry;
						Entry.Name = Binding.BindCount > 1 ? Name + "[" + std::to_string(ArrayIndex) + "]" : Name;
						Entry.Slot = Binding.BindPoint + ArrayIndex;
						Entry.BindCount = Binding.BindCount;

						for (size_t Index = 0; Index < NextLayout.Samplers.size(); ++Index)
						{
							if (NextLayout.Samplers[Index].Name == Entry.Name || NextLayout.Samplers[Index].Slot == Entry.Slot)
							{
								std::cout << "Sampler 이름 또는 슬롯이 중복됨: " + Name << "\n";
								return E_FAIL;
							}
						}
						NextLayout.Samplers.push_back(Entry);
					}
				}

				if (Binding.Type == D3D_SIT_TEXTURE)
				{
					FPTextureType TextureType = Texture2D;
					if (Binding.Dimension == D3D_SRV_DIMENSION_TEXTURE2D) { TextureType = Texture2D; }
					else if (Binding.Dimension == D3D_SRV_DIMENSION_TEXTURE2DARRAY) { TextureType = Texture2DArray; }
					else { std::cout <<  "지원하지 않는 텍스처 차원: " + Name << "\n"; return E_FAIL; }

					FPReflectionResourceInfo Entry;
					Entry.Name = Name;
					Entry.Slot = Binding.BindPoint;
					Entry.BindCount = Binding.BindCount;
					// 배열 요소마다 AssetName을 개별 관리할 수 있도록 항목을 나눔
					for (unsigned int ArrayIndex = 0; ArrayIndex < Binding.BindCount; ++ArrayIndex)
					{
						FPReflectionResourceInfo Entry;
						Entry.Name = Binding.BindCount > 1 ? Name + "[" + std::to_string(ArrayIndex) + "]" : Name;
						Entry.TextureType = TextureType;
						Entry.Slot = Binding.BindPoint + ArrayIndex;
						Entry.BindCount = Binding.BindCount;

						for (size_t Index = 0; Index < NextLayout.Resources.size(); ++Index)
						{
							if (NextLayout.Resources[Index].Name == Entry.Name || NextLayout.Resources[Index].Slot == Entry.Slot)
							{
								std::cout << "Texture 이름 또는 슬롯이 중복됨: " + Entry.Name << "\n";
								return E_FAIL;
							}
						}
						NextLayout.Resources.push_back(Entry);
					}
				}
			}
		}
	}

	// JSON이 요청한 CB가 최적화로 제거되었거나 다른 Stage에 있으면 오류
	for (std::set < std::pair<FPShaderType, std::string> >::const_iterator It = ManagedCBs.begin(); It != ManagedCBs.end(); ++It)
	{
		if (FoundCBs.count(*It) == 0)
		{
			std::cout << "컴파일된 셰이더에서 찾지 못한 상수 버퍼: " + It->second << "\n";
			return E_FAIL;

		}
	}

	*(OutReflectionData) = std::move(NextLayout);
	*(OutVS) = std::move(NextVS);
	*(OutPS) = std::move(NextPS);

	return hr;
}

HRESULT RenderingDevice::AppendMaterialParameter(ID3D11ShaderReflectionType* ReflectedType, const std::string& MemberName, size_t RelativeOffset, size_t ConstantBufferSize, size_t BufferStart, FPShaderType Stage, const std::string& ConstantBufferName, FPShaderReflectionData& Layout)
{
	if (ReflectedType == nullptr) return E_FAIL;
	HRESULT hr = S_OK;

	D3D11_SHADER_TYPE_DESC TypeDesc = {};
	hr = ReflectedType->GetDesc(&TypeDesc);
	if (FAILED(hr)) return E_FAIL;

	//구조체인 경우
	if (TypeDesc.Class == D3D_SVC_STRUCT)
	{
		// 구조체 배열은 별도 패킹 규칙이 필요하므로 이번 지원 대상에서 제외
		if (TypeDesc.Elements != 0)
		{
			return E_FAIL;
		}

		for (UINT MemberIndex = 0; MemberIndex < TypeDesc.Members; ++MemberIndex)
		{
			ID3D11ShaderReflectionType* ChildType = ReflectedType->GetMemberTypeByIndex(MemberIndex);
			const char* ChildName = ReflectedType->GetMemberTypeName(MemberIndex);
			if (ChildType == nullptr || ChildName == nullptr) return E_FAIL;

			D3D11_SHADER_TYPE_DESC ChildDesc = {};
			hr = ChildType->GetDesc(&ChildDesc);
			if (FAILED(hr)) return E_FAIL;

			if (RelativeOffset > ConstantBufferSize || ChildDesc.Offset > ConstantBufferSize - RelativeOffset)
			{
				std::cout << "구조체 멤버 오프셋이 상수 버퍼 범위를 벗어남: " + MemberName << "\n";
				return E_FAIL;
			}

			//재귀를 통해 구조체 안에 각 요소를 등록
			hr = AppendMaterialParameter(ChildType, MemberName + "." + ChildName, RelativeOffset + ChildDesc.Offset, ConstantBufferSize, BufferStart, Stage, ConstantBufferName, Layout);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
		}
		return S_OK;
	}

	//flot , float2 , float3, floa4 형식외의 형식은 넘어감
	if ((TypeDesc.Class != D3D_SVC_SCALAR && TypeDesc.Class != D3D_SVC_VECTOR) || TypeDesc.Type != D3D_SVT_FLOAT || TypeDesc.Rows != 1 || TypeDesc.Columns == 0 || TypeDesc.Columns > 4)
	{
		std::cout << "MaterialConstBuffer에서 지원하지 않는 Parameter: " + MemberName << "\n";
		return E_FAIL;
	}

	// HLSL의 float 계열 배열은 각 원소가 16바이트 경계에서 시작함
	const size_t DataSize = sizeof(float) * TypeDesc.Columns;
	const unsigned int ArrayCount = TypeDesc.Elements == 0 ? 1u : TypeDesc.Elements;
	const size_t ArrayStride = TypeDesc.Elements == 0 ? 0u : 16u;

	if (RelativeOffset > ConstantBufferSize || DataSize > ConstantBufferSize - RelativeOffset)
	{
		std::cout << "Parameter가 상수 버퍼 크기를 초과함 " + MemberName << "\n";
		return E_FAIL;
	}

	//배열인 경우
	if (TypeDesc.Elements != 0)
	{
		// 첫 원소 이외의 모든 원소도 상수 버퍼 범위 안인지 확인
		const size_t Remaining = ConstantBufferSize - RelativeOffset - DataSize;
		if (static_cast<size_t>(ArrayCount - 1) > Remaining / ArrayStride)
		{
			std::cout << "배열 Parameter가 상수 버퍼 크기를 초과함: " + MemberName << "\n";
			return E_FAIL;
		}
	}

	for (size_t Index = 0; Index < Layout.Parameters.size(); ++Index)
	{
		const FPReflectionParameterInfo& Existing = Layout.Parameters[Index];
		if (Existing.Type == Stage && Existing.ConstantBufferName == ConstantBufferName && Existing.ParameterName == MemberName)
		{
			std::cout << "중복 Parameter: " + ConstantBufferName + "." + MemberName << "\n";
			return E_FAIL;
		}
	}

	FPReflectionParameterInfo Entry;
	Entry.Type = Stage;
	Entry.ConstantBufferName = ConstantBufferName;
	Entry.ParameterName = MemberName;
	Entry.BufferOffset = BufferStart + RelativeOffset;
	Entry.Components = TypeDesc.Columns;
	Entry.ArrayCount = ArrayCount;
	Entry.ArrayStride = ArrayStride;
	Layout.Parameters.push_back(Entry);

	return hr;
}

HRESULT RenderingDevice::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* Ed, DWORD Num, ID3DBlob* InVSCode, ID3D11InputLayout** ReturnLayout)
{

	HRESULT hr = S_OK;

	//정점 입력구조 객체 생성
	//함께 사용될 셰이더(컴파일된 바이너리 코드)가 필요

	hr = Device->CreateInputLayout(Ed, Num, InVSCode->GetBufferPointer(), InVSCode->GetBufferSize(), ReturnLayout);
	assert(SUCCEEDED(hr));
	return hr;
}

void RenderingDevice::OMSetDepthStencilState(void* InDepthStencilState)
{
	ID3D11DepthStencilState* DepthStencilState = static_cast<ID3D11DepthStencilState*>(InDepthStencilState);
	DeviceContext->OMSetDepthStencilState(DepthStencilState, 0);
}

HRESULT RenderingDevice::CreateSamplers()
{
	HRESULT hr = S_OK;
	//샘플러 정보 설정
	D3D11_SAMPLER_DESC sd = {};
	
	//기본값 설정
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.BorderColor[0] = 1;
	sd.BorderColor[1] = 1;
	sd.BorderColor[2] = 1;
	sd.BorderColor[3] = 1;

	//텍스처 어드레스 모드
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//Clamp
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	//Point
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Point_Sampler_CLAMP]);
	if (FALSE(hr)) { std::cout << "Filter_Point_Sampler_CLAMP 샘플러 생성 실패\n"; }

	//Bi Linear
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Bi_Linear_Sampler_CLAMP]);
	if (FALSE(hr)) { std::cout << "Filter_Bi_Linear_Sampler_CLAMP 샘플러 생성 실패\n"; }

	//Tri Linear (기본값)
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Tri_Linear_Sampler_CLAMP]);
	if (FALSE(hr)) { std::cout << "Filter_Tri_Linear_Sampler_CLAMP 샘플러 생성 실패\n"; }


	//Anistropy
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = dwAF;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Anistropy_Linear_Sampler_CLAMP]);
	if (FALSE(hr)) { std::cout << "Filter_Anistropy_Linear_Sampler_CLAMP 샘플러 생성 실패\n"; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//Wrap
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//Point
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Point_Sampler_Wrap]);
	if (FALSE(hr)) { std::cout << "Filter_Point_Sampler_Wrap 샘플러 생성 실패\n"; }

	//Bi Linear
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Bi_Linear_Sampler_Wrap]);
	if (FALSE(hr)) { std::cout << "Filter_Bi_Linear_Sampler_Wrap 샘플러 생성 실패\n"; }

	//Tri Linear (기본값)
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Tri_Linear_Sampler_Wrap]);
	if (FALSE(hr)) { std::cout << "Filter_Tri_Linear_Sampler_Wrap 샘플러 생성 실패\n"; }


	//Anistropy
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = dwAF;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Anistropy_Linear_Sampler_Wrap]);
	if (FALSE(hr)) { std::cout << "Filter_Anistropy_Linear_Sampler_Wrap 샘플러 생성 실패\n"; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//Mirror
	sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

	//Point
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Point_Sampler_Mirror]);
	if (FALSE(hr)) { std::cout << "Filter_Point_Sampler_Mirror 샘플러 생성 실패\n"; }

	//Bi Linear
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Bi_Linear_Sampler_Mirror]);
	if (FALSE(hr)) { std::cout << "Filter_Bi_Linear_Sampler_Mirror 샘플러 생성 실패\n"; }

	//Tri Linear (기본값)
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Tri_Linear_Sampler_Mirror]);
	if (FALSE(hr)) { std::cout << "Filter_Tri_Linear_Sampler_Mirror 샘플러 생성 실패\n"; }


	//Anistropy
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = dwAF;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Anistropy_Linear_Sampler_Mirror]);
	if (FALSE(hr)) { std::cout << "Filter_Anistropy_Linear_Sampler_Mirror 샘플러 생성 실패\n"; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//Mirror-Once
	sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

	//Point
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Point_Sampler_Mirror_Once]);
	if (FALSE(hr)) { std::cout << "Filter_Point_Sampler_Mirror_Once 샘플러 생성 실패\n"; }

	//Bi Linear
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Bi_Linear_Sampler_Mirror_Once]);
	if (FALSE(hr)) { std::cout << "Filter_Bi_Linear_Sampler_Mirror_Once 샘플러 생성 실패\n"; }

	//Tri Linear (기본값)
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Tri_Linear_Sampler_Mirror_Once]);
	if (FALSE(hr)) { std::cout << "Filter_Tri_Linear_Sampler_Mirror_Once 샘플러 생성 실패\n"; }


	//Anistropy
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = dwAF;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Anistropy_Linear_Sampler_Mirror_Once]);
	if (FALSE(hr)) { std::cout << "Filter_Anistropy_Linear_Sampler_Mirror_Once 샘플러 생성 실패\n"; }
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//Border BasicSet
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.BorderColor[0] = 1;
	sd.BorderColor[1] = 1;
	sd.BorderColor[2] = 1;
	sd.BorderColor[3] = 1;

	//Point
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Point_Sampler_Border]);
	if (FALSE(hr)) { std::cout << "Filter_Point_Sampler_Border 샘플러 생성 실패\n"; }

	//Bi Linear
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Bi_Linear_Sampler_Border]);
	if (FALSE(hr)) { std::cout << "Filter_Bi_Linear_Sampler_Border 샘플러 생성 실패\n"; }

	//Tri Linear (기본값)
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Tri_Linear_Sampler_Border]);
	if (FALSE(hr)) { std::cout << "Filter_Tri_Linear_Sampler_Border 샘플러 생성 실패\n"; }


	//Anistropy
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = dwAF;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Anistropy_Linear_Sampler_Border]);
	if (FALSE(hr)) { std::cout << "Filter_Anistropy_Linear_Sampler_Border 샘플러 생성 실패\n"; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//BorderColor Red
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.BorderColor[0] = 1;
	sd.BorderColor[1] = 0;
	sd.BorderColor[2] = 0;
	sd.BorderColor[3] = 1;

	//Point
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Point_Sampler_BorderColor_RED]);
	if (FALSE(hr)) { std::cout << "Filter_Point_Sampler_BorderColor_RED 샘플러 생성 실패\n"; }

	//Bi Linear
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Bi_Linear_Sampler_BorderColor_RED]);
	if (FALSE(hr)) { std::cout << "Filter_Bi_Linear_Sampler_BorderColor_RED 샘플러 생성 실패\n"; }

	//Tri Linear (기본값)
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Tri_Linear_Sampler_BorderColor_RED]);
	if (FALSE(hr)) { std::cout << "Filter_Tri_Linear_Sampler_BorderColor_RED 샘플러 생성 실패\n"; }


	//Anistropy
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = dwAF;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Anistropy_Linear_Sampler_BorderColor_RED]);
	if (FALSE(hr)) { std::cout << "Filter_Anistropy_Linear_Sampler_BorderColor_RED 샘플러 생성 실패\n"; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//BorderColor Blue
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.BorderColor[0] = 0;
	sd.BorderColor[1] = 0;
	sd.BorderColor[2] = 1;
	sd.BorderColor[3] = 1;

	//Point
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Point_Sampler_BorderColor_BLUE]);
	if (FALSE(hr)) { std::cout << "Filter_Point_Sampler_BorderColor_BLUE 샘플러 생성 실패\n"; }

	//Bi Linear
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Bi_Linear_Sampler_BorderColor_BLUE]);
	if (FALSE(hr)) { std::cout << "Filter_Bi_Linear_Sampler_BorderColor_BLUE 샘플러 생성 실패\n"; }

	//Tri Linear (기본값)
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Tri_Linear_Sampler_BorderColor_BLUE]);
	if (FALSE(hr)) { std::cout << "Filter_Tri_Linear_Sampler_BorderColor_BLUE 샘플러 생성 실패\n"; }


	//Anistropy
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = dwAF;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Anistropy_Linear_Sampler_BorderColor_BLUE]);
	if (FALSE(hr)) { std::cout << "Filter_Anistropy_Linear_Sampler_BorderColor_BLUE 샘플러 생성 실패\n"; }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//BorderColor Green
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.BorderColor[0] = 0;
	sd.BorderColor[1] = 1;
	sd.BorderColor[2] = 0;
	sd.BorderColor[3] = 1;

	//Point
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Point_Sampler_BorderColor_GREEN]);
	if (FALSE(hr)) { std::cout << "Filter_Point_Sampler_BorderColor_GREEN 샘플러 생성 실패\n"; }

	//Bi Linear
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Bi_Linear_Sampler_BorderColor_GREEN]);
	if (FALSE(hr)) { std::cout << "Filter_Bi_Linear_Sampler_BorderColor_GREEN 샘플러 생성 실패\n"; }

	//Tri Linear (기본값)
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Tri_Linear_Sampler_BorderColor_GREEN]);
	if (FALSE(hr)) { std::cout << "Filter_Tri_Linear_Sampler_BorderColor_GREEN 샘플러 생성 실패\n"; }


	//Anistropy
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = dwAF;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Anistropy_Linear_Sampler_BorderColor_GREEN]);
	if (FALSE(hr)) { std::cout << "Filter_Anistropy_Linear_Sampler_BorderColor_GREEN 샘플러 생성 실패\n"; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//BorderColor Magenta
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.BorderColor[0] = 1;
	sd.BorderColor[1] = 0;
	sd.BorderColor[2] = 1;
	sd.BorderColor[3] = 1;

	//Point
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Point_Sampler_BorderColor_MAGENTA]);
	if (FALSE(hr)) { std::cout << "Filter_Point_Sampler_BorderColor_MAGENTA 샘플러 생성 실패\n"; }

	//Bi Linear
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Bi_Linear_Sampler_BorderColor_MAGENTA]);
	if (FALSE(hr)) { std::cout << "Filter_Bi_Linear_Sampler_BorderColor_MAGENTA 샘플러 생성 실패\n"; }

	//Tri Linear (기본값)
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Tri_Linear_Sampler_BorderColor_MAGENTA]);
	if (FALSE(hr)) { std::cout << "Filter_Tri_Linear_Sampler_BorderColor_MAGENTA 샘플러 생성 실패\n"; }


	//Anistropy
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = dwAF;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	hr = Device->CreateSamplerState(&sd, &SamplerState[Filter_Anistropy_Linear_Sampler_BorderColor_MAGENTA]);
	if (FALSE(hr)) { std::cout << "Filter_Anistropy_Linear_Sampler_BorderColor_MAGENTA 샘플러 생성 실패\n"; }

	return hr;
}

HRESULT RenderingDevice::PSSetSamplers(UINT Slot, UINT NumSamplers, FPSamplerType SamplerType)
{
	DeviceContext->PSSetSamplers(Slot, NumSamplers, &SamplerState[SamplerType]);
	return S_OK;
}

///////////////////////////////////
//
// 블랜드 상태 객체 생성 우선 RT0 백버퍼만 사용
//
HRESULT RenderingDevice::CreateBlendStateObject()
{
	//블랜딩 상태 객체 구성 옵션
	D3D11_BLEND_DESC bd = {};
	
	D3D11_RENDER_TARGET_BLEND_DESC rtb = {};

	//NULL 초기화용
	BlendStateObject[BS_NULL_] = NULL;

	//색상 혼합 개체 #0  기본
	rtb.BlendEnable = FALSE;	//블렌딩 동작 결정. 기본값은 FALSE (OFF)

	//색상 성분 혼합 : Color Blending. (기본값)
	rtb.BlendOp = D3D11_BLEND_OP_ADD;
	rtb.SrcBlend = D3D11_BLEND_ONE;
	rtb.DestBlend = D3D11_BLEND_ZERO;

	//알파 성분 혼합 : Alpha Blending. (기본값)
	rtb.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtb.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtb.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtb.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	//렌더타겟 0번 혼합 상태 결정
	bd.RenderTarget[0] = rtb;
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = FALSE;

	Device->CreateBlendState(&bd, &BlendStateObject[BS_DEFAULT]);
	
	//색상 혼합 개체 #1 "AB"
	rtb.BlendEnable = TRUE;
	rtb.BlendOp = D3D11_BLEND_OP_ADD;

	//색상 혼합 : 알파 기준 혼합
	//최종색 = Src.Color * Src.a + Dest.Color * (1 - Src.a)
	rtb.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtb.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	bd.RenderTarget[0] = rtb;
	bd.AlphaToCoverageEnable  = TRUE;
	//bd.IndependentBlendEnable = FALSE;

	Device->CreateBlendState(&bd, &BlendStateObject[BS_ADD]);

	return S_OK;
}
 
HRESULT RenderingDevice::OMSetBlendState(FPBlendingStateType BlendType)
{
	DeviceContext->OMSetBlendState(BlendStateObject[BlendType], NULL, 0xFFFFFFFF);
	return S_OK;
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
	std::wstring WidePath = std::filesystem::path(FPPathManager::Get().GetEngineAssetPath("Font/굴림9k.sfont").c_str()).wstring();
	const TCHAR* Filename = WidePath.c_str();

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

void RenderingDevice::RasterStateCreate()
{
	//상태객체 1 : 기본 렌더링 상태 객체
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;		//삼각형 색상 채우기(기본값)
	rd.CullMode = D3D11_CULL_NONE;		//컬링 없음. (기본값은 컬링 Back)
	rd.FrontCounterClockwise = true;	//이하 기본값
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;	//MS-AA 적용.★
	rd.AntialiasedLineEnable = true;	//Line 그리기시 AA 적용 여부 (옵션)★

	//레스터라이저 객체 생성
	Device->CreateRasterizerState(&rd, &RState[RS_SOLID]);

	
	//상태객체 2 : 와이어 프레임 그리기
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE;
	Device->CreateRasterizerState(&rd, &RState[RS_WIREFRAME]);

	//상태객체 3 : 컬링 ON CCW
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	Device->CreateRasterizerState(&rd, &RState[RS_CULLBACK]);

	//상태객체 4 : 와이어 프레임 + 컬링 ON CCW
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	Device->CreateRasterizerState(&rd, &RState[RS_WIRECULLBACK]);
}

void RenderingDevice::RasterStateRelease()
{
	for (int i = 0; i < RS_MAX_; i++)
	{
		SafeRelease(RState[i]);
	}
}

#define CheckRMode(k, v) if((k)) RMode |= (v); else RMode &= ~(v);

void RenderingDevice::UpdateRSSetState(bool isFill, bool isCull)
{
	//렌더링 모드 체크
	CheckRMode(!isFill, RM_WIREFRAME);
	CheckRMode(isCull, RM_CULLBACK);

	ID3D11RasterizerState* SelectedState = nullptr;
	//레스터 모드 전환
	switch (RMode)
	{
	default:
	case RM_SOLID:
		SelectedState = RState[RS_SOLID];
		break;

	case RM_WIREFRAME:
		SelectedState = RState[RS_WIREFRAME];
		break;

	case RM_CULLBACK:
		SelectedState = RState[RS_CULLBACK];
		break;

	case RM_WIREFRAME | RM_CULLBACK:
		SelectedState = RState[RS_WIRECULLBACK];
		break;
	}
	DeviceContext->RSSetState(SelectedState);
}

void RenderingDevice::VSSetShader(void* VS)
{
	ID3D11VertexShader* pVS = static_cast<ID3D11VertexShader*>(VS);
	DeviceContext->VSSetShader(pVS, nullptr, 0);
}

void RenderingDevice::PSSetShader(void* PS)
{
	ID3D11PixelShader* pPS = static_cast<ID3D11PixelShader*>(PS);
	DeviceContext->PSSetShader(pPS, nullptr, 0);
}


void RenderingDevice::IASetVertexBuffers(UINT StartSlot, UINT NumBuffers, void* VertexBuffer, UINT* Strides, UINT* Offsets)
{
	ID3D11Buffer* VB = static_cast<ID3D11Buffer*>(VertexBuffer);
	DeviceContext->IASetVertexBuffers(StartSlot, NumBuffers, &VB, Strides, Offsets);
}

void RenderingDevice::IASetIndexBuffers(void* IndexBuffer, UINT Offset)
{
	ID3D11Buffer* IB = static_cast<ID3D11Buffer*>(IndexBuffer);
	DeviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT ,Offset);
}


void RenderingDevice::IASetInputLayout(void* InputLayout)
{
	ID3D11InputLayout* pInputLayout = static_cast<ID3D11InputLayout*>(InputLayout);
	DeviceContext->IASetInputLayout(pInputLayout);
}

void RenderingDevice::IASetPrimitiveTopology(Topology topo)
{
	D3D_PRIMITIVE_TOPOLOGY D3DTopo = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	switch (topo)
	{
		case Topology::TRIANGLELIST:
			D3DTopo = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;

		case Topology::TRIANGLESTRIP:
			D3DTopo = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;

		case Topology::LINELIST:
			D3DTopo = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
	}
	DeviceContext->IASetPrimitiveTopology(D3DTopo);
}

//장치 제거
HRESULT RenderingDevice::DeviceFinalize()
{
	//장치 상태 리셋 : 제거 전에 초기화 필수 (메모리 누수 방지)
	if (DeviceContext) DeviceContext->ClearState();

	BackBufferRenderTargetView->Release();
	SwapChain->Release();
	DeviceContext->Release();
	Device->Release();

	return S_OK;
}

HRESULT RenderingDevice::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	DeviceContext->Draw(VertexCount, StartVertexLocation);
	return S_OK;
}

HRESULT RenderingDevice::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	DeviceContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
	return S_OK;
}

//WIC Texture Load
HRESULT RenderingDevice::CreateWICTextureFromFile(std::string& FilePath, void** TextureResourceView)
{
	HRESULT hr = S_OK;
	ID3D11Device* DevicePointer = Device.Get();
	ID3D11DeviceContext* DeviceContextPointer = DeviceContext.Get();
	ID3D11ShaderResourceView* TextureRV = nullptr;

	std::wstring TexturePath(FilePath.begin(), FilePath.end());
	//hr = DirectX::CreateWICTextureFromFile(DevicePointer, TexturePath.c_str(), nullptr, &TextureRV);

	hr = DirectX::CreateWICTextureFromFileEx(DevicePointer, DeviceContextPointer, TexturePath.c_str(), 0, 				//Mip-map 텍스처 생성.
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		D3D11_RESOURCE_MISC_GENERATE_MIPS,		//밉멥 생성 플래그.★
		WIC_LOADER_DEFAULT,
		nullptr, &TextureRV);
	if (FAILED(hr))
	{
		std::cout << FilePath << " 텍스처 로드 실패" << "\n";
		return hr;
	}
	*TextureResourceView = TextureRV;
	return hr;
}

//DDS Texture Load
HRESULT RenderingDevice::CreateDDSTextureFromFile(std::string& FilePath, void** TextureResourceView)
{
	HRESULT hr = S_OK;
	ID3D11Device* DevicePointer = Device.Get();
	ID3D11DeviceContext* DeviceContextPointer = DeviceContext.Get();
	ID3D11ShaderResourceView* TextureRV = nullptr;

	std::wstring TexturePath(FilePath.begin(), FilePath.end());
	hr = DirectX::CreateDDSTextureFromFile(DevicePointer, DeviceContextPointer, TexturePath.c_str(), nullptr, &TextureRV);


	if (FAILED(hr))
	{
		std::cout << FilePath << " 텍스처 로드 실패" << "\n";
		return hr;
	}
	*TextureResourceView = TextureRV;
	return hr;
}

//Textre 정보 획득
HRESULT RenderingDevice::GetTextureDesc(void* TextureResourceView, void** Desc)
{
	HRESULT hr = S_OK;

	ID3D11ShaderResourceView* TextureRV = static_cast<ID3D11ShaderResourceView*>(TextureResourceView);

	//셰이더 리소스 뷰 정보 획득
	D3D11_SHADER_RESOURCE_VIEW_DESC ResourceDesc;
	TextureRV->GetDesc(&ResourceDesc);

	//텍스처 정보 획득
	ID3D11Resource* Res = nullptr;
	TextureRV->GetResource(&Res);

	ID3D11Texture2D* Tex = nullptr;
	Res->QueryInterface<ID3D11Texture2D>(&Tex);
	if (Tex)
	{
		D3D11_TEXTURE2D_DESC* TextureDesc = new D3D11_TEXTURE2D_DESC();

		Tex->GetDesc(TextureDesc);

		*Desc = TextureDesc;

		Tex->Release();
	}
	return hr;
}

HRESULT RenderingDevice::CheckTextureArrayData(UINT Texture2DArrayWidth, UINT Texture2DArrayHeight, std::vector<void*>* CheckTexture, std::vector<UINT>* TextureMiplevels, std::vector<void*>* TextureSources, UINT* MinMiplevels, void** OutBaseDesc, void** OutBaseSRVDesc)
{
	D3D11_TEXTURE2D_DESC BaseDesc = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC BaseSRVDesc = {};

	UINT MinMipLevels = (std::numeric_limits<UINT>::max)();

	for (int i = 0; i < (*CheckTexture).size(); ++i)
	{
		ID3D11ShaderResourceView* SourceSRV = static_cast<ID3D11ShaderResourceView*>((*(CheckTexture))[i]);

		if (!SourceSRV) { std::cout << "[CheckTextureArrayData] : 유효하지 않은 SRV" << "\n"; return E_FAIL; }

		//GPU 리소스 획득
		ID3D11Resource* Resource;
		SourceSRV->GetResource(&Resource);

		//SRV에서 실제 리소스 획득
		ID3D11Texture2D* SourceTexture;
		
		HRESULT hr = Resource->QueryInterface(__uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&SourceTexture));

		// GetResource()로 획득한 참조 해제
		Resource->Release();
		Resource = nullptr;

		if (FAILED(hr))
		{
			std::cout << "[CheckTextureArrayData] : Texture2D 리소스가 아님!" << "\n";
			return hr;
		}

		//Texture 정보 조회

		D3D11_TEXTURE2D_DESC Desc = {};
		SourceTexture->GetDesc(&Desc);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SourceSRV->GetDesc(&SRVDesc);

		//MSAA 미지원
		if (Desc.SampleDesc.Count != 1)
		{
			std::cout << "[CheckTextureArrayData] : Texture Array는 NSAA 미지원" << "\n";
			return E_FAIL;
		}

		//단일 Texture 2D만 허용
		if (Desc.ArraySize != 1 || SRVDesc.ViewDimension != D3D11_SRV_DIMENSION_TEXTURE2D)
		{
			std::cout << "[CheckTextureArrayData] : Texture Array는 단일 Texture2D만 지원" << "\n";
			return E_FAIL;
		}

		//해상도 확인
		if (Desc.Width != Texture2DArrayWidth || Desc.Height != Texture2DArrayHeight)
		{
			std::cout << "[CheckTextureArrayData] : Texture가 TextureArray2D 해상도와 디릅니다!" << "\n";
			return E_FAIL;
		}

		//MipMap 검증
		if (Desc.MipLevels == 0)
		{
			std::cout << "[CheckTextureArrayData] : Texture가 정상적인 Texture가 아닙니다!" << "\n";
			return E_FAIL;
		}

		if (i == 0)
		{
			// 첫 번째 텍스처를 기준으로 설정
			BaseDesc = Desc;
			BaseSRVDesc = SRVDesc;
		}
		else
		{
			// 포맷 검증
			if (Desc.Format != BaseDesc.Format || SRVDesc.Format != BaseSRVDesc.Format)
			{
				std::cout << "[CheckTextureArrayData] : Texture끼리 Format이 다릅니다." << "\n";
				return E_FAIL;
			}
		}

		//최소 MipLevels 계산
		MinMipLevels = (std::min)(MinMipLevels, Desc.MipLevels);
		TextureMiplevels->push_back(Desc.MipLevels);
		TextureSources->push_back(SourceTexture);
	}

	D3D11_TEXTURE2D_DESC* BucketBaseDesc = new D3D11_TEXTURE2D_DESC();
	D3D11_SHADER_RESOURCE_VIEW_DESC* BucketBaseSRVDesc = new D3D11_SHADER_RESOURCE_VIEW_DESC();

	(*BucketBaseDesc) = BaseDesc;
	(*BucketBaseSRVDesc) = BaseSRVDesc;

	(*OutBaseDesc) = BucketBaseDesc;
	(*OutBaseSRVDesc) = BucketBaseSRVDesc;
	*(MinMiplevels) = MinMipLevels;
}

HRESULT RenderingDevice::CreateTextureArray2D(UINT Width, UINT Height, UINT ArraySize, std::vector<void*>* TextureSources, std::vector<UINT>* SourceMipLevels, void* BaseDesc, void* BaseSRVDesc, UINT MinMiplevels, void** Texture2DArraySRV)
{
	D3D11_TEXTURE2D_DESC* InBaseDesc = static_cast<D3D11_TEXTURE2D_DESC*>(BaseDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC* InBaseSRVDesc = static_cast<D3D11_SHADER_RESOURCE_VIEW_DESC*>(BaseSRVDesc);

	ID3D11Texture2D* NewTexture = nullptr;
	ID3D11ShaderResourceView* NewSRV = nullptr;

	D3D11_TEXTURE2D_DESC ArrayDesc = {};

	ArrayDesc.Width = Width;
	ArrayDesc.Height = Height;

	// 최소 공통 MipLevels 사용
	ArrayDesc.MipLevels = MinMiplevels;

	ArrayDesc.ArraySize = ArraySize;
	ArrayDesc.Format = InBaseDesc->Format;

	ArrayDesc.SampleDesc.Count = 1;
	ArrayDesc.SampleDesc.Quality = 0;

	ArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	ArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	ArrayDesc.CPUAccessFlags = 0;
	ArrayDesc.MiscFlags = 0;

	HRESULT hr = Device->CreateTexture2D(&ArrayDesc,nullptr, &NewTexture);

	if (FAILED(hr))
	{
		std::cout << "[CreateTextureArray2D] : Texture2D 만들기 실패!" << "\n";
		return hr;
	}

	// 각 Slice의 공통 MipLevels 복사
	for (UINT Slice = 0; Slice < ArraySize; ++Slice)
	{
		for (UINT Mip = 0; Mip < MinMiplevels; ++Mip)
		{
			// 대상 Texture2DArray의 서브리소스
			UINT DestSubresource = D3D11CalcSubresource(
				Mip,
				Slice,
				ArrayDesc.MipLevels
			);

			// 원본 Texture2D의 서브리소스
			UINT SrcSubresource = D3D11CalcSubresource(
				Mip,
				0,
				(*SourceMipLevels)[Slice]
			);

			DeviceContext->CopySubresourceRegion(
				NewTexture,
				DestSubresource,
				0, 0, 0,
				static_cast<ID3D11Resource*>((*TextureSources)[Slice]),
				SrcSubresource,
				nullptr
			);
		}
	}

	//Texture2DArray SRV 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC ViewDesc = {};

	ViewDesc.Format = InBaseSRVDesc->Format;

	ViewDesc.ViewDimension =
		D3D11_SRV_DIMENSION_TEXTURE2DARRAY;

	ViewDesc.Texture2DArray.MostDetailedMip = 0;

	// 최소 MipLevels만 노출
	ViewDesc.Texture2DArray.MipLevels = MinMiplevels;

	ViewDesc.Texture2DArray.FirstArraySlice = 0;
	ViewDesc.Texture2DArray.ArraySize = ArraySize;

	hr = Device->CreateShaderResourceView(NewTexture,&ViewDesc,&NewSRV);

	if (FAILED(hr))
	{
		std::cout << "[CreateTextureArray2D] : SRV 만들기 실패!" << "\n";
		return hr;
	}

	(*Texture2DArraySRV) = NewSRV;
}

HRESULT RenderingDevice::PSSetShaderResources(UINT Slot, UINT NumViews, void* ShaderResourceView)
{
	ID3D11ShaderResourceView* pShaderResourceView = static_cast<ID3D11ShaderResourceView*>(ShaderResourceView);
	ID3D11ShaderResourceView* const* ppShaderResourceView = &pShaderResourceView;
	DeviceContext->PSSetShaderResources(Slot, NumViews, ppShaderResourceView);
	return S_OK;
}

HRESULT RenderingDevice::DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
{
	DeviceContext->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
	return S_OK;
}



//RenderTargetView, Depth 관련 객체 해제
void RenderingDevice::ResetRTVandDepthObj()
{
	BackBufferRenderTargetView.Reset();
}


//SwapChainBuffer 크기 재설정
void RenderingDevice::ResizeSwapChainBuffer(int Width, int Height)
{
	SwapChainWidth = Width;
	SwapChainHeight = Height;

	SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
}

void RenderingDevice::DisplayModeSize(int Width, int Height)
{
	DisplayMode.Width = Width;
	DisplayMode.Height = Height;
}