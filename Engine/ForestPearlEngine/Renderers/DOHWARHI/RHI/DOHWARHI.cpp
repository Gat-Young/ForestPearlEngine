#include "DOHWARHI.h"
#include "../../DOHWA/DOHWA/Dohwa.h"

//DOHWARHI 생성 함수
FPRHI* CreateRHI(UINT DeviceVersion)
{
	return new DOHWARHI(DeviceVersion);
}

///////////////////////////////////////////////
//
// 타입 변환기

//FPRHIRESOURCETYPE To DOHWARESOURCETYPE 타입 변환기
DOHWARESOURCETYPE ChangeDOHWARESOURCETYPE(FPRHIRESOURCETYPE ResourceType)
{
    switch (ResourceType)
    {
    case FPRHIRTYPE_SURFACE: return DOHWARTYPE_SURFACE;
    case FPRHIRTYPE_VOLUME:	return DOHWARTYPE_VOLUME;
    case FPRHIRTYPE_TEXTURE:	return DOHWARTYPE_TEXTURE;
    case FPRHIRTYPE_VOLUMETEXTURE:	return DOHWARTYPE_VOLUMETEXTURE;
    case FPRHIRTYPE_CUBETEXTURE:	return DOHWARTYPE_CUBETEXTURE;
    case FPRHIRTYPE_VERTEXBUFFER:	return DOHWARTYPE_VERTEXBUFFER;
    case FPRHIRTYPE_INDEXBUFFER:	return DOHWARTYPE_INDEXBUFFER;
    case FPRHIRTYPE_FORCE_DWORD:	return DOHWARTYPE_FORCE_DWORD;
    default: return DOHWARTYPE_FORCE_DWORD;

    }
}

//FPRHIPOOL To DOHWAPOOL 타입 변환기
DOHWAPOOL ChangeDOHWAPOOL(FPRHIPOOL MemoryPool)
{
    switch (MemoryPool)
    {
    case FPRHIPOOL_DEFAULT: return DOHWAPOOL_DEFAULT;
    case FPRHIPOOL_MANAGED:	return DOHWAPOOL_MANAGED;
    case FPRHIPOOL_SYSTEMMEM:	return DOHWAPOOL_SYSTEMMEM;
    case FPRHIPOOL_SCRATCH:	return DOHWAPOOL_SCRATCH;
    case FPRHIPOOL_FORCE_DWORD:	return DOHWAPOOL_FORCE_DWORD;
    default: return DOHWAPOOL_FORCE_DWORD;

    }
}

//FPRHIFORMAT To DOHWAFORMAT 타입 변환기
DOHWAFORMAT ChangeDOHWAFORMAT(FPRHIFORMAT FPRHIFormat)
{
    switch (FPRHIFormat)
    {
    case FPRHIFMT_UNKNOWN:   return DOHWAFMT_UNKNOWN;

    case FPRHIFMT_R8G8B8:   return DOHWAFMT_R8G8B8;
    case FPRHIFMT_A8R8G8B8:   return DOHWAFMT_A8R8G8B8;
    case FPRHIFMT_X8R8G8B8:   return DOHWAFMT_X8R8G8B8;
    case FPRHIFMT_R5G6B5:   return DOHWAFMT_R5G6B5;
    case FPRHIFMT_X1R5G5B5: return DOHWAFMT_X1R5G5B5;
    case FPRHIFMT_A1R5G5B5:   return DOHWAFMT_A1R5G5B5;
    case FPRHIFMT_A4R4G4B4:   return DOHWAFMT_A4R4G4B4;
    case FPRHIFMT_R3G3B2:   return DOHWAFMT_R3G3B2;
    case FPRHIFMT_A8:   return DOHWAFMT_A8;
    case FPRHIFMT_A8R3G3B2:   return DOHWAFMT_A8R3G3B2;
    case FPRHIFMT_X4R4G4B4:   return DOHWAFMT_X4R4G4B4;
    case FPRHIFMT_A2B10G10R10:   return DOHWAFMT_A2B10G10R10;
    case FPRHIFMT_A8B8G8R8:   return DOHWAFMT_A8B8G8R8;

    case FPRHIFMT_D16_LOCKABLE:   return DOHWAFMT_D16_LOCKABLE;
    case FPRHIFMT_D32:   return DOHWAFMT_D32;
    case FPRHIFMT_D24S8:   return DOHWAFMT_D24S8;
    case FPRHIFMT_D24X8:   return DOHWAFMT_D24X8;
    case FPRHIFMT_D16:   return DOHWAFMT_D16;

    case FPRHIFMT_VERTEXDATA:   return DOHWAFMT_VERTEXDATA;
    case FPRHIFMT_INDEX16:   return DOHWAFMT_INDEX16;
    case FPRHIFMT_INDEX32:   return DOHWAFMT_INDEX32;

    case FPRHIFMT_FORCE_DWORD:   return DOHWAFMT_FORCE_DWORD;

    default: return DOHWAFMT_FORCE_DWORD;
    }
}


//FPRHIPRESENT_PARAMETERS To DOHWAPRESENT_PARAMETERS 타입 변환기
void ChangeDOHWAPRESENT_PARAMETERS(DOHWAPRESENT_PARAMETERS& DohwaPram, FPRHIPRESENT_PARAMETERS* pPresentationParameters)
{
    DohwaPram.Width = pPresentationParameters->BackBufferWidth;
    DohwaPram.Height = pPresentationParameters->BackBufferHeight;
    DohwaPram.BackBuffercnt = pPresentationParameters->BackBufferCount;
    DohwaPram.Windowed = pPresentationParameters->Windowed;
}

//FPRHIUsages To DOHWAUsage 타입 변환기
long ChangeDOHWAUsage(long FPRHIUsage)
{
    switch (FPRHIUsage)
    {
    case FPRHIUSAGE_RENDERTARGET: return DOHWAUSAGE_RENDERTARGET;
    case FPRHIUSAGE_DEPTHSTENCIL:	return DOHWAUSAGE_DEPTHSTENCIL;
    case FPRHIUSAGE_DYNAMIC:	return DOHWAUSAGE_DYNAMIC;

    case FPRHIUSAGE_AUTOGENMIPMAP:	return DOHWAUSAGE_AUTOGENMIPMAP;
    case FPRHIUSAGE_DMAP:	return DOHWAUSAGE_DMAP;

    case FPRHIUSAGE_WRITEONLY:	return DOHWAUSAGE_WRITEONLY;
    case FPRHIUSAGE_SOFTWAREPROCESSING:	return DOHWAUSAGE_SOFTWAREPROCESSING;
    }
}

//FPRHICREATE To DOHWACREATE 타입 변환기
DWORD ChangeDOHWACREATE(DWORD FPRHICreate)
{
    switch (FPRHICreate)
    {
    case FPRHICREATE_SOFTWARE_VERTEXPROCESSING: return DOHWACREATE_SOFTWARE_VERTEXPROCESSING;
    case FPRHICREATE_HARDWARE_VERTEXPROCESSING:	return DOHWACREATE_HARDWARE_VERTEXPROCESSING;
    case FPRHICREATE_MIXED_VERTEXPROCESSING:	return DOHWACREATE_MIXED_VERTEXPROCESSING;

    case FPRHICREATE_MULTITHREADED:	return DOHWACREATE_MULTITHREADED;
    }
}

//FPRHICOLOR To COLORREF 타입 변환기
COLORREF ChangeCOLORREF(FPRHICOLOR FPRHIColor)
{
    DWORD A = FPRHIColor & 0xff000000;
    DWORD R = FPRHIColor & 0x00ff0000;
    DWORD G = FPRHIColor & 0x0000ff00;
    DWORD B = FPRHIColor & 0x000000ff;

    return ((COLORREF)(((B) << 16) | (G) | ((R) >> 16)));
}

/////////////////////////////////////////////
//
// class DOHWADeviceImpl 구현
//
////////////////////////////////////////////
class DOHWADeviceImpl
{
    private :
        std::unique_ptr<IDohwaDevice9> DohwaDevice;

    public:
        DOHWADeviceImpl();
        ~DOHWADeviceImpl();

        std::unique_ptr<IDohwaDevice9>* GetDevice() { return &(DohwaDevice); };
        int BeginScene();
        int EndScene();
        int Clear(FPRHICOLOR col);
        int Present();
};

DOHWADeviceImpl::DOHWADeviceImpl()
{

}

DOHWADeviceImpl::~DOHWADeviceImpl()
{

}

int DOHWADeviceImpl::BeginScene()
{
    DohwaDevice->BeginScene();
    return DOHWA_OK;
}

int DOHWADeviceImpl::EndScene()
{
    DohwaDevice->EndScene();
    return DOHWA_OK;
}

int DOHWADeviceImpl::Clear(FPRHICOLOR col)
{
    DohwaDevice->Clear(ChangeCOLORREF(col));
    return DOHWA_OK;
}

int DOHWADeviceImpl::Present()
{
    DohwaDevice->Present();
    return DOHWA_OK;
}





/////////////////////////////////////////////
//
// class DOHWAImpl 구현
//
////////////////////////////////////////////
class DOHWAImpl
{
    private:
        std::unique_ptr<IDohwa> DohwaObj;

    public:
        DOHWAImpl(UINT DeviceVersion);
        ~DOHWAImpl();

        //Com객체 반환
        IDohwa* GetDohwa() { return DohwaObj.get(); };

        //Device 생성
        HRESULT CreateDevice(HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, std::unique_ptr<IDohwaDevice9>* ppReturnedDeviceInterface);
};

DOHWAImpl::DOHWAImpl(UINT DeviceVersion)
{
    IDohwa* IDohwaObject = DohwaCreate9(static_cast<DWORD>(DeviceVersion));
    DohwaObj.reset(IDohwaObject);
}

DOHWAImpl::~DOHWAImpl()
{
}

HRESULT DOHWAImpl::CreateDevice(HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, std::unique_ptr<IDohwaDevice9>* ppReturnedDeviceInterface)
{
    IDohwaDevice9* DeviceObj = nullptr;

    DOHWAPRESENT_PARAMETERS DohwaParam;
    ZeroMemory(&DohwaParam, sizeof(DohwaParam));
    ChangeDOHWAPRESENT_PARAMETERS(DohwaParam, pPresentationParameters);
    DohwaObj->CreateDevice(hFocusWindow, &DohwaParam, ChangeDOHWACREATE(BehaviorFlags), &DeviceObj);

    ppReturnedDeviceInterface->reset(DeviceObj);
    return S_OK;
}



/////////////////////////////////////////////////////
//
// DOHWARHI 구현부
//
////////////////////////////////////////////////////

DOHWARHI::DOHWARHI(UINT DeviceVersion) : DOHWAimpl(std::make_unique<DOHWAImpl>(DeviceVersion))
{
}

DOHWARHI::~DOHWARHI()
{
}

HRESULT DOHWARHI::CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface)
{
    DOHWARHIDevice* DOHWARHIDeviceObj = new DOHWARHIDevice();

    DOHWAimpl->CreateDevice(hFocusWindow, BehaviorFlags, pPresentationParameters, DOHWARHIDeviceObj->DOHWADeviceimpl->GetDevice());

    *ppReturnedDeviceInterface = DOHWARHIDeviceObj;
    return E_NOTIMPL;
}

/////////////////////////////////////////////////////
//
// DOHWARHIDevice 구현부
//
////////////////////////////////////////////////////

DOHWARHIDevice::DOHWARHIDevice():DOHWADeviceimpl(std::make_unique<DOHWADeviceImpl>())
{
}

DOHWARHIDevice::~DOHWARHIDevice()
{
}

HRESULT DOHWARHIDevice::BeginScene()
{
    DOHWADeviceimpl->BeginScene();
    return E_NOTIMPL;
}

HRESULT DOHWARHIDevice::Clear(DWORD Count, const FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil)
{
    DOHWADeviceimpl->Clear(Color);
    return E_NOTIMPL;
}

HRESULT DOHWARHIDevice::EndScene()
{
    DOHWADeviceimpl->EndScene();
    return E_NOTIMPL;
}

HRESULT DOHWARHIDevice::Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
    DOHWADeviceimpl->Present();
    return E_NOTIMPL;
}
