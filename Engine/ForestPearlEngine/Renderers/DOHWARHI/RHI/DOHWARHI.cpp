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

//FPRHICOLOR To COLORREF 타입 변환기
COLORREF ChangeCOLORREF(FPRHICOLOR FPRHIColor)
{
    DWORD A = FPRHIColor & 0xff000000;
    DWORD R = FPRHIColor & 0x00ff0000;
    DWORD G = FPRHIColor & 0x0000ff00;
    DWORD B = FPRHIColor & 0x000000ff;

    return ((COLORREF)(((B) << 16) | (G) | ((R) >> 16)));
}

//FPRHIPRESENT_PARAMETERS To DOHWAPRESENT_PARAMETERS 타입 변환기
void ChangeDOHWAPRESENT_PARAMETERS(DOHWAPRESENT_PARAMETERS& DohwaPram, FPRHIPRESENT_PARAMETERS* pPresentationParameters)
{
    DohwaPram.Width = pPresentationParameters->BackBufferWidth;
    DohwaPram.Height = pPresentationParameters->BackBufferHeight;
    DohwaPram.BackBuffercnt = pPresentationParameters->BackBufferCount;
    DohwaPram.Windowed = pPresentationParameters->Windowed;
}

//FPRHIFVF To DOHWAFVF 타입 변환기
DWORD ChangeDOHWAFVF_FORMAT(DWORD FPRHIFvf)
{

    //위치 타입만 뽑아내기
    DWORD FPRHIPosType = FPRHIFvf & FPRHIFVF_POSITION_MASK;

    DWORD D3DPosType;

    switch (FPRHIPosType)
    {
    case  FPRHIFVF_RESERVED0: D3DPosType = DOHWAFVF_XY; break;
    case  FPRHIFVF_XYZ:       D3DPosType = DOHWAFVF_XYZ; break;
    case  FPRHIFVF_XYZRHW:    D3DPosType = DOHWAFVF_XYZRHW; break;
    default:                  D3DPosType = 0x000; break;
    }

    //법선 벡터 여부
    DWORD DXNormal = ((FPRHIFvf & FPRHIFVF_NORMAL) == DOHWAFVF_NORMAL) ? DOHWAFVF_NORMAL : 0x000;

    // 점 프리미티브 크기 여부
    DWORD DXPsize = 0x000;

    //Diffuse Color 여부
    DWORD DXDiffuse = ((FPRHIFvf & FPRHIFVF_DIFFUSE) == DOHWAFVF_DIFFUSE) ? DOHWAFVF_DIFFUSE : 0x000;

    //Specular Color 여부
    DWORD DXSpecular = ((FPRHIFvf & FPRHIFVF_SPECULAR) == DOHWAFVF_SPECULAR) ? DOHWAFVF_SPECULAR : 0x000;


    //텍스처 좌표 개수 뽑아내기
    DWORD FPRHITextCount = (FPRHIFvf & FPRHIFVF_TEXCOUNT_MASK);

    DWORD DXTextCount;

    switch (FPRHIPosType)
    {
    case  FPRHIFVF_TEX1:     DXTextCount = DOHWAFVF_TEX1; break;
    case  FPRHIFVF_TEX2:     DXTextCount = DOHWAFVF_TEX2; break;
    default:                 DXTextCount = 0x000; break;
    }

    //LASTBETA
    DWORD FPRHILastBeta = (FPRHIFvf ^ 0x0fff);

    DWORD DXLastBeta;

    switch (FPRHIPosType)
    {
    default:                             DXLastBeta = 0x000; break;
    }

    return (D3DPosType | DXNormal | DXPsize | DXDiffuse | DXSpecular | DXTextCount | DXLastBeta);
}

//FPRHIRENDERSTATETYPE To DOHWARENDERSTATETYPE 타입 변환기
DOHWARENDERSTATETYPE ChangeDOHWARENDERSTATETYPE(FPRHIRENDERSTATETYPE FPRHIRenderStateType)
{
    switch (FPRHIRenderStateType)
    {
    case FPRHIRS_ZENABLE:                   return DOHWARS_ZENABLE;
    case FPRHIRS_FILLMODE:                  return DOHWARS_FILLMODE;
    case FPRHIRS_ZWRITEENABLE:              return DOHWARS_ZWRITEENABLE;
    case FPRHIRS_ALPHATESTENABLE:           return DOHWARS_ALPHATESTENABLE;
    case FPRHIRS_CULLMODE:                  return DOHWARS_CULLMODE;
    case FPRHIRS_ALPHABLENDENABLE:          return DOHWARS_ALPHABLENDENABLE;
    case FPRHIRS_FOGENABLE:                 return DOHWARS_FOGENABLE;
    case FPRHIRS_SPECULARENABLE:            return DOHWARS_SPECULARENABLE;
    case FPRHIRS_LIGHTING:                  return DOHWARS_LIGHTING;
    case FPRHIRS_AMBIENT:                   return DOHWARS_AMBIENT;

    case FPRHIRS_FORCE_DWORD:               return DOHWARS_MAX_;
    default:                                return DOHWARS_MAX_;
    }
}

//FPRHIFILLMODE To DOHWAFILLMODE 타입 변환기
DOHWAFILLMODE ChangeDOHWAFILLMODE(DWORD FPRHIFillMode)
{
    switch (FPRHIFillMode)
    {
    case FPRHIFILL_POINT:           return DOHWAFILL_POINT;
    case FPRHIFILL_WIREFRAME:       return DOHWAFILL_WIREFRAME;
    case FPRHIFILL_SOLID:           return DOHWAFILL_SOLID;
    case FPRHIFILL_FORCE_DWORD:     return DOHWAFILL_SOLID;

    default:                        return DOHWAFILL_SOLID;
    }
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

//FPRHIPRIMITIVETYPE To DOHWAPRIMITIVETYPE 타입 변환기
DOHWAPRIMITIVETYPE ChangeDOHWAPRIMITIVETYPE(FPRHIPRIMITIVETYPE FPRHIPrmititiveType)
{
    switch (FPRHIPrmititiveType)
    {
    case FPRHIPT_POINTLIST:       return DOHWAPT_POINTLIST;
    case FPRHIPT_LINELIST:        return DOHWAPT_LINELIST;
    case FPRHIPT_LINESTRIP:       return DOHWAPT_LINESTRIP;
    case FPRHIPT_TRIANGLELIST:    return DOHWAPT_TRIANGLELIST;
    case FPRHIPT_TRIANGLESTRIP:   return DOHWAPT_TRIANGLESTRIP;
    case FPRHIPT_TRIANGLEFAN:     return DOHWAPT_TRIANGLEFAN;
    case FPRHIPT_FORCE_DWORD:     return DOHWAPT_FORCE_DWORD;

    default:                        return DOHWAPT_FORCE_DWORD;
    }
}
/////////////////////////////////////////////
//
// class DOHWAVertexBufferImpl 구현
//
////////////////////////////////////////////
class DOHWAVertexBufferImpl
{
    private:
        std::unique_ptr<IDohwaVertexBuffer9> VertexBuffer;

    public:
        DOHWAVertexBufferImpl();
        ~DOHWAVertexBufferImpl();

        //VertexBuffer 객체 반환
        std::unique_ptr<IDohwaVertexBuffer9>* GetVertexBuffer() { return &(VertexBuffer); }

        HRESULT Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
        HRESULT Unlock();
};

DOHWAVertexBufferImpl::DOHWAVertexBufferImpl()
{

}

DOHWAVertexBufferImpl::~DOHWAVertexBufferImpl()
{

}

HRESULT DOHWAVertexBufferImpl::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
    VertexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
    return S_OK;
}

HRESULT DOHWAVertexBufferImpl::Unlock()
{
    VertexBuffer->Unlock();
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

        std::unique_ptr<IDohwaDevice9>* GetDevice() { return &(DohwaDevice); }

        int BeginScene();
        int EndScene();
        int Clear(FPRHICOLOR col);
        int Present();
        
        HRESULT GetDC(HDC* phdc);
        HRESULT ReleaseDC(HDC hdc);

        HRESULT SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value);
        HRESULT GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue);


        HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, std::unique_ptr<IDohwaVertexBuffer9>* ppVertexBuffer, HANDLE* pSharedHandle);
        HRESULT SetStreamSource(UINT StreamNumber, std::unique_ptr<IDohwaVertexBuffer9>& pStreamData, UINT OffsetInBytes, UINT Stride);
        HRESULT SetFVF(DWORD FVF);
        HRESULT DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
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


HRESULT DOHWADeviceImpl::DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    DohwaDevice->DrawPrimitive(ChangeDOHWAPRIMITIVETYPE(PrimitiveType), StartVertex, PrimitiveCount);
    return S_OK;
}


HRESULT DOHWADeviceImpl::GetDC(HDC* phdc)
{
    *phdc = DohwaDevice->GetRT();
    return DOHWA_OK;
}

HRESULT DOHWADeviceImpl::ReleaseDC(HDC hdc)
{
    return DOHWA_OK;
}

HRESULT DOHWADeviceImpl::SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value)
{
    DOHWARENDERSTATETYPE DOHWARenderStateType = ChangeDOHWARENDERSTATETYPE(State);
    if (DOHWARenderStateType == DOHWARS_CULLMODE)
    {
        //Device->SetRenderState(DOHWARenderStateType, ChangeDOHWACULL(Value));
    }

    if (DOHWARenderStateType == DOHWARS_FILLMODE)
    {
        DohwaDevice->SetRenderState(DOHWARenderStateType, ChangeDOHWAFILLMODE(Value));
    }
    return S_OK;
}

HRESULT DOHWADeviceImpl::GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue)
{
    return S_OK;
}

HRESULT DOHWADeviceImpl::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, std::unique_ptr <IDohwaVertexBuffer9>* ppVertexBuffer, HANDLE* pSharedHandle)
{
    IDohwaVertexBuffer9* VertexBufferObj = nullptr;

    HRESULT hr = DohwaDevice->CreateVertexBuffer(Length, ChangeDOHWAUsage(Usage), ChangeDOHWAFVF_FORMAT(FVF), ChangeDOHWAPOOL(Pool), &VertexBufferObj, pSharedHandle);

    ppVertexBuffer->reset(VertexBufferObj);

    return S_OK;
}


HRESULT DOHWADeviceImpl::SetStreamSource(UINT StreamNumber, std::unique_ptr<IDohwaVertexBuffer9>& pStreamData, UINT OffsetInBytes, UINT Stride)
{
    DohwaDevice->SetStreamSource(StreamNumber, pStreamData.get(), OffsetInBytes, Stride);
    return S_OK;
}

HRESULT DOHWADeviceImpl::SetFVF(DWORD FVF)
{
    DohwaDevice->SetFVF(ChangeDOHWAFVF_FORMAT(FVF));
    return S_OK;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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

HRESULT DOHWARHIDevice::GetDC(HDC* phdc)
{
    DOHWADeviceimpl->GetDC(phdc);
    return E_NOTIMPL;
}

HRESULT DOHWARHIDevice::ReleaseDC(HDC hdc)
{
    DOHWADeviceimpl->ReleaseDC(hdc);
    return S_OK;
}

HRESULT DOHWARHIDevice::DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    DOHWADeviceimpl->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
    return S_OK;
}

HRESULT DOHWARHIDevice::SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value)
{
    DOHWADeviceimpl->SetRenderState(State, Value);
    return S_OK;
}

HRESULT DOHWARHIDevice::GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue)
{
    DOHWADeviceimpl->GetRenderState(State, pValue);
    return S_OK;
}

HRESULT DOHWARHIDevice::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, FPRHIVertexBuffer** ppVertexBuffedr, HANDLE* pSharedHandle)
{
    DOHWAVertexBuffer* DOHWAVB = new DOHWAVertexBuffer();
    DOHWADeviceimpl->CreateVertexBuffer(Length, Usage, FVF, Pool, DOHWAVB->DOHWAVertexBufferimpl->GetVertexBuffer(), pSharedHandle);
    *ppVertexBuffedr = DOHWAVB;
    return S_OK;
}

HRESULT DOHWARHIDevice::SetStreamSource(UINT StreamNumber, FPRHIVertexBuffer* pStreamData, UINT OffsetInBytes, UINT Stride)
{
    DOHWAVertexBuffer* DOHWAVB = dynamic_cast<DOHWAVertexBuffer*>(pStreamData);
    DOHWADeviceimpl->SetStreamSource(StreamNumber, *(DOHWAVB->DOHWAVertexBufferimpl->GetVertexBuffer()), OffsetInBytes, Stride);
    return S_OK;
}

HRESULT DOHWARHIDevice::SetFVF(DWORD FVF)
{
    DOHWADeviceimpl->SetFVF(FVF);
    return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//
// DOHWAVertexBuffer 구현부
//
////////////////////////////////////////////////////

DOHWAVertexBuffer::DOHWAVertexBuffer() :DOHWAVertexBufferimpl(std::make_unique<DOHWAVertexBufferImpl>())
{
}

DOHWAVertexBuffer::~DOHWAVertexBuffer()
{
}

HRESULT DOHWAVertexBuffer::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
    DOHWAVertexBufferimpl->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
    return S_OK;
}

HRESULT DOHWAVertexBuffer::Unlock()
{
    DOHWAVertexBufferimpl->Unlock();
    return S_OK;
}
