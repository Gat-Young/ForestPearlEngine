#include "DXRHI.h"
#include <d3d9.h>
#include <wrl/client.h>

template<typename T>
using Comptr = Microsoft::WRL::ComPtr<T>;


/////////////////////////
// 전역 함수
// 생성 함수
FPRHI* CreateRHI(UINT DeviceVersion)
{
    return new DXRHI(DeviceVersion);
}

/////////////////////////
// 전역 함수
// 타입 변환기
//FPRHIDEVTYPE To D3DDEVTYPE 타입 변환기
D3DDEVTYPE ChangeD3DDEVTYPE(FPRHIDEVTYPE DeviceType)
{
    switch (DeviceType)
    {
    case FPRHIDEVTYPE_HAL: return D3DDEVTYPE_HAL;
    case FPRHIDEVTYPE_REF:	return D3DDEVTYPE_REF;
    case FPRHIDEVTYPE_SW:	return D3DDEVTYPE_SW;
    case FPRHIDEVTYPE_NULLREF:	return D3DDEVTYPE_NULLREF;
    case FPRHIDEVTYPE_FORCE_DWORD:	return D3DDEVTYPE_FORCE_DWORD;
    }
}

//FPRHIPRESENT_PARAMETERS To D3DPRESENT_PARAMETERS 타입 변환기
void ChangeD3DPRESENT_PARAMETERS(D3DPRESENT_PARAMETERS& D3D, FPRHIPRESENT_PARAMETERS* pPresentationParameters)
{
    D3D.BackBufferWidth = pPresentationParameters->BackBufferWidth;
    D3D.BackBufferHeight = pPresentationParameters->BackBufferHeight;
    D3D.BackBufferFormat = ChangeD3DFORMAT(pPresentationParameters->BackBufferFormat);
    D3D.BackBufferCount = pPresentationParameters->BackBufferCount;

    D3D.MultiSampleType = ChangeD3DFORMAT(pPresentationParameters->MultiSampleType);
    D3D.MultiSampleQuality = pPresentationParameters->MultiSampleQuality;

    D3D.SwapEffect = ChangeD3DSWAPEFFECT(pPresentationParameters->SwapEffect);
    D3D.hDeviceWindow = pPresentationParameters->hDeviceWindow;
    D3D.Windowed = pPresentationParameters->Windowed;
    D3D.EnableAutoDepthStencil = pPresentationParameters->EnableAutoDepthStencil;
    D3D.AutoDepthStencilFormat = ChangeD3DFORMAT(pPresentationParameters->AutoDepthStencilFormat);
    D3D.Flags = pPresentationParameters->Flags;

    D3D.FullScreen_RefreshRateInHz = pPresentationParameters->FullScreen_RefreshRateInHz;
    D3D.PresentationInterval = pPresentationParameters->PresentationInterval;

}

//FPRHIFORMAT To D3DFORMAT 타입 변환기
D3DFORMAT ChangeD3DFORMAT(FPRHIFORMAT FPRHIFormat)
{
    switch (FPRHIFormat)
    {
    case FPRHIFMT_UNKNOWN:   return D3DFMT_UNKNOWN;

    case FPRHIFMT_R8G8B8:   return D3DFMT_R8G8B8;
    case FPRHIFMT_A8R8G8B8:   return D3DFMT_A8R8G8B8;
    case FPRHIFMT_X8R8G8B8:   return D3DFMT_X8R8G8B8;
    case FPRHIFMT_R5G6B5:   return D3DFMT_R5G6B5;
    case FPRHIFMT_X1R5G5B5:
    case FPRHIFMT_A1R5G5B5:   return D3DFMT_A1R5G5B5;
    case FPRHIFMT_A4R4G4B4:   return D3DFMT_A4R4G4B4;
    case FPRHIFMT_R3G3B2:   return D3DFMT_R3G3B2;
    case FPRHIFMT_A8:   return D3DFMT_A8;
    case FPRHIFMT_A8R3G3B2:   return D3DFMT_A8R3G3B2;
    case FPRHIFMT_X4R4G4B4:   return D3DFMT_X4R4G4B4;
    case FPRHIFMT_A2B10G10R10:   return D3DFMT_A2B10G10R10;
    case FPRHIFMT_A8B8G8R8:   return D3DFMT_A8B8G8R8;
    case FPRHIFMT_X8B8G8R8:   return D3DFMT_X8B8G8R8;
    case FPRHIFMT_G16R16:   return D3DFMT_G16R16;
    case FPRHIFMT_A2R10G10B10:   return D3DFMT_A2R10G10B10;
    case FPRHIFMT_A16B16G16R16:   return D3DFMT_A16B16G16R16;

    case FPRHIFMT_A8P8:   return D3DFMT_A8P8;
    case FPRHIFMT_P8:   return D3DFMT_P8;

    case FPRHIFMT_L8:   return D3DFMT_L8;
    case FPRHIFMT_A8L8:   return D3DFMT_A8L8;
    case FPRHIFMT_A4L4:   return D3DFMT_A4L4;

    case FPRHIFMT_V8U8:   return D3DFMT_V8U8;
    case FPRHIFMT_L6V5U5:   return D3DFMT_L6V5U5;
    case FPRHIFMT_X8L8V8U8:   return D3DFMT_X8L8V8U8;
    case FPRHIFMT_Q8W8V8U8:   return D3DFMT_Q8W8V8U8;
    case FPRHIFMT_V16U16:   return D3DFMT_V16U16;
    case FPRHIFMT_A2W10V10U10:   return D3DFMT_A2W10V10U10;

    case FPRHIFMT_UYVY:   return D3DFMT_UYVY;
    case FPRHIFMT_R8G8_B8G8:   return D3DFMT_R8G8_B8G8;
    case FPRHIFMT_YUY2:   return D3DFMT_YUY2;
    case FPRHIFMT_G8R8_G8B8:   return D3DFMT_G8R8_G8B8;
    case FPRHIFMT_DXT1:   return D3DFMT_DXT1;
    case FPRHIFMT_DXT2:   return D3DFMT_DXT2;
    case FPRHIFMT_DXT3:   return D3DFMT_DXT3;
    case FPRHIFMT_DXT4:   return D3DFMT_DXT4;
    case FPRHIFMT_DXT5:   return D3DFMT_DXT5;

    case FPRHIFMT_D16_LOCKABLE:   return D3DFMT_D16_LOCKABLE;
    case FPRHIFMT_D32:   return D3DFMT_D32;
    case FPRHIFMT_D15S1:   return D3DFMT_D15S1;
    case FPRHIFMT_D24S8:   return D3DFMT_D24S8;
    case FPRHIFMT_D24X8:   return D3DFMT_D24X8;
    case FPRHIFMT_D24X4S4:   return D3DFMT_D24X4S4;
    case FPRHIFMT_D16:   return D3DFMT_D16;

    case FPRHIFMT_D32F_LOCKABLE:   return D3DFMT_D32F_LOCKABLE;
    case FPRHIFMT_D24FS8:   return D3DFMT_D24FS8;

    case FPRHIFMT_L16:   return D3DFMT_L16;

    case FPRHIFMT_VERTEXDATA:   return D3DFMT_VERTEXDATA;
    case FPRHIFMT_INDEX16:   return D3DFMT_INDEX16;
    case FPRHIFMT_INDEX32:   return D3DFMT_INDEX32;

    case FPRHIFMT_Q16W16V16U16:   return D3DFMT_Q16W16V16U16;

    case FPRHIFMT_MULTI2_ARGB8:   return D3DFMT_MULTI2_ARGB8;

        // Floating point surface formats

        // s10e5 formats (16-bits per channel)
    case FPRHIFMT_R16F:   return D3DFMT_R16F;
    case FPRHIFMT_G16R16F:   return D3DFMT_G16R16F;
    case FPRHIFMT_A16B16G16R16F:   return D3DFMT_A16B16G16R16F;

        // IEEE s23e8 formats (32-bits per channel)
    case FPRHIFMT_R32F:   return D3DFMT_R32F;
    case FPRHIFMT_G32R32F:   return D3DFMT_G32R32F;
    case FPRHIFMT_A32B32G32R32F:   return D3DFMT_A32B32G32R32F;

    case FPRHIFMT_CxV8U8:   return D3DFMT_CxV8U8;

    case FPRHIFMT_FORCE_DWORD:   return D3DFMT_FORCE_DWORD;
    }
}

//FPRHIMULTISAMPLE_TYPE To D3DMULTISAMPLE_TYPE 타입 변환기
D3DMULTISAMPLE_TYPE ChangeD3DFORMAT(FPRHIMULTISAMPLE_TYPE FPRHIMultisampletype)
{
    switch (FPRHIMultisampletype)
    {
    case FPRHIMULTISAMPLE_NONE:   return D3DMULTISAMPLE_NONE;
    case FPRHIMULTISAMPLE_NONMASKABLE:   return D3DMULTISAMPLE_NONMASKABLE;
    case FPRHIMULTISAMPLE_2_SAMPLES:   return D3DMULTISAMPLE_2_SAMPLES;
    case FPRHIMULTISAMPLE_3_SAMPLES:   return D3DMULTISAMPLE_3_SAMPLES;
    case FPRHIMULTISAMPLE_4_SAMPLES:   return D3DMULTISAMPLE_4_SAMPLES;
    case FPRHIMULTISAMPLE_5_SAMPLES:   return D3DMULTISAMPLE_5_SAMPLES;
    case FPRHIMULTISAMPLE_6_SAMPLES:   return D3DMULTISAMPLE_6_SAMPLES;
    case FPRHIMULTISAMPLE_7_SAMPLES:   return D3DMULTISAMPLE_7_SAMPLES;
    case FPRHIMULTISAMPLE_8_SAMPLES:   return D3DMULTISAMPLE_8_SAMPLES;
    case FPRHIMULTISAMPLE_9_SAMPLES:   return D3DMULTISAMPLE_9_SAMPLES;
    case FPRHIMULTISAMPLE_10_SAMPLES:   return D3DMULTISAMPLE_10_SAMPLES;
    case FPRHIMULTISAMPLE_11_SAMPLES:   return D3DMULTISAMPLE_11_SAMPLES;
    case FPRHIMULTISAMPLE_12_SAMPLES:   return D3DMULTISAMPLE_12_SAMPLES;
    case FPRHIMULTISAMPLE_13_SAMPLES:   return D3DMULTISAMPLE_13_SAMPLES;
    case FPRHIMULTISAMPLE_14_SAMPLES:   return D3DMULTISAMPLE_14_SAMPLES;
    case FPRHIMULTISAMPLE_15_SAMPLES:   return D3DMULTISAMPLE_15_SAMPLES;
    case FPRHIMULTISAMPLE_16_SAMPLES:   return D3DMULTISAMPLE_16_SAMPLES;

    case FPRHIMULTISAMPLE_FORCE_DWORD:   return D3DMULTISAMPLE_FORCE_DWORD;
    }
}

//FPRHISWAPEFFECT To D3DSWAPEFFECT 타입 변환기
D3DSWAPEFFECT ChangeD3DSWAPEFFECT(FPRHISWAPEFFECT FPRHISwapeffect)
{
    switch (FPRHISwapeffect)
    {
    case FPRHISWAPEFFECT_DISCARD:   return D3DSWAPEFFECT_DISCARD;
    case FPRHISWAPEFFECT_FLIP:   return D3DSWAPEFFECT_FLIP;
    case FPRHISWAPEFFECT_COPY:   return D3DSWAPEFFECT_COPY;

    case FPRHISWAPEFFECT_FORCE_DWORD:   return D3DSWAPEFFECT_FORCE_DWORD;
    }
}

//FPRHIRECT To D3DRECT 타입 변환기
void ChangeD3DRECT(const FPRHIRECT* FPRHIRect, D3DRECT& D3DRect)
{
    D3DRect.x1 = FPRHIRect->x1;
    D3DRect.y1 = FPRHIRect->y1;
    D3DRect.x2 = FPRHIRect->x2;
    D3DRect.y2 = FPRHIRect->y2;
}

//FPRHICOLOR To D3DCOLOR 타입 변환기
void ChangeD3DCOLOR(FPRHICOLOR& FPRHIColor, D3DCOLOR& D3DColor)
{
    D3DColor = FPRHIColor;
}


/////////////////////////
// DXImpl
// DX pimpl 패턴 적용
class DXRHI::DXImpl
{
    private:
        Comptr<IDirect3D9> D3D;

    public:
        DXImpl(UINT DeviceVersion) {};
        ~DXImpl() = default;
        
        //Com객체 반환
        IDirect3D9** GetD3D() { return D3D.GetAddressOf(); };

        //Device 생성
        HRESULT CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);
};

DXRHI::DXImpl::DXImpl(UINT DeviceVersion)
{
    //DX 객체 만들기
    D3D.Attach(Direct3DCreate9(DeviceVersion));

    //생성 되었는지 체크 필요
}

//DXDevice 생성
HRESULT DXRHI::DXImpl::CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
    D3DDEVTYPE D3DDevice = ChangeD3DDEVTYPE(DeviceType);
    D3DPRESENT_PARAMETERS D3DPP;
    ZeroMemory(&D3DPP, sizeof(D3DPP));
    ChangeD3DPRESENT_PARAMETERS(D3DPP, pPresentationParameters);

    HRESULT res = D3D->CreateDevice(Adapter, D3DDevice, hFocusWindow, BehaviorFlags, &D3DPP, ppReturnedDeviceInterface);
    if (FAILED(res))
    {
        return E_FAIL;
    }

    return S_OK;

}





//////////////////////
// DXRHI
DXRHI::DXRHI(UINT DeviceVersion) : DeviceVersion(DeviceVersion), DXimpl(std::make_unique<DXImpl>(DeviceVersion)) {}

DXRHI::~DXRHI() = default;

//DXDevice 생성
HRESULT DXRHI::CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface)
{
    DXRHIDevice* Device = new DXRHIDevice();
    HRESULT res = DXimpl->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, Device->DXDeviceimpl->GetDevice());
    if (FAILED(res))
    {
        return E_FAIL;
    }

    *ppReturnedDeviceInterface = Device;

    return S_OK;
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////
// DXDeviceImpl
class DXRHIDevice::DXDeviceImpl
{
    private :
        Comptr<IDirect3DDevice9> Device;

    public :
        DXDeviceImpl();
        ~DXDeviceImpl();
        //Com객체 반환
        IDirect3DDevice9** GetDevice() { return Device.GetAddressOf(); };
        HRESULT BeginScene();
        HRESULT Clear(DWORD Count, const FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil);
        HRESULT EndScene();
        HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
};

DXRHIDevice::DXDeviceImpl::DXDeviceImpl() = default;

DXRHIDevice::DXDeviceImpl::~DXDeviceImpl() = default;

HRESULT DXRHIDevice::DXDeviceImpl::BeginScene()
{
    Device->BeginScene();
    return S_OK;
}

HRESULT DXRHIDevice::DXDeviceImpl::Clear(DWORD Count, const FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil)
{
    D3DRECT D3Drect;
    ChangeD3DRECT(pRects, D3Drect);
    D3DCOLOR D3DColor;
    ChangeD3DCOLOR(Color, D3DColor);
    Device->Clear(Count, &D3Drect, Flags, D3DColor, Z, Stencil);
    return S_OK;
}

HRESULT DXRHIDevice::DXDeviceImpl::EndScene()
{
    Device->EndScene();
    return S_OK;
}

HRESULT DXRHIDevice::DXDeviceImpl::Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
    Device->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    return S_OK;
}



//////////////////////
// DXDevice

DXRHIDevice::DXRHIDevice():DXDeviceimpl(std::make_unique<DXDeviceImpl>())
{
}

DXRHIDevice::~DXRHIDevice() = default;

HRESULT DXRHIDevice::BeginScene()
{
    DXDeviceimpl->BeginScene();
	return S_OK;
}

HRESULT DXRHIDevice::Clear(DWORD Count, const FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil)
{
    DXDeviceimpl->Clear(Count, pRects, Flags, Color, Z, Stencil);
	return S_OK;
}

HRESULT DXRHIDevice::EndScene()
{
    DXDeviceimpl->EndScene();
	return S_OK;
}


HRESULT DXRHIDevice::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
    DXDeviceimpl->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return S_OK;
}
