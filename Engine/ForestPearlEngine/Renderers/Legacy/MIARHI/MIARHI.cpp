#include "MIARHI.h"
#include "../MIA/MIA.h"
#include <iostream>

/////////////////////////////////////////////////////////////
//
// 전역함수
//

FPRHI* CreateRHI(UINT DeviceVersion)
{
	return new MCRHI(DeviceVersion);
}

////////////////////////////////
// 파라미터 변환기
//FPRHIPRESENT_PARAMETERS To B3MPRESENT_PARAMETERS 타입 변환기
void ChangeToB3MPRESENT_PARAMETERS(B3MPRESENT_PARAMETERS& B3M, FPRHIPRESENT_PARAMETERS* FP)
{
	B3M.Width = FP->BackBufferWidth;
	B3M.Height = FP->BackBufferHeight;
	B3M.BackBuffercnt = FP->BackBufferCount;
	B3M.Windowed = FP->Windowed;
}

void ChangeToB3MPRIMITIVETYPE(B3MPRIMITIVETYPE& B3M, FPRHIPRIMITIVETYPE FP)
{
	switch (FP)
	{
	case FPRHIPRIMITIVETYPE::FPRHIPT_LINELIST :
		B3M = B3MPRIMITIVETYPE::B3MPT_LINELIST;
		break;
	case FPRHIPRIMITIVETYPE::FPRHIPT_TRIANGLELIST :
		B3M = B3MPRIMITIVETYPE::B3MPT_TRIANGLELIST;
		break;
	default:
		std::cout << "ChangeToB3MPRIMITIVETYPE :: No Value!" << "\n";
		break;
	}
}

void ChangeToB3MRENDERSTATETYPE(B3MRENDERSTATETYPE& B3M, FPRHIRENDERSTATETYPE FP)
{
	switch (FP)
	{
	case FPRHIRENDERSTATETYPE::FPRHIRS_FILLMODE:
		B3M = B3MRENDERSTATETYPE::B3MRS_FILLMODE;
		break;
	case FPRHIRENDERSTATETYPE::FPRHIRS_CULLMODE:
		B3M = B3MRENDERSTATETYPE::B3MRS_CULLMODE;
		break;
		
	default:
		std::cout << "ChangeToB3MRENDERSTATETYPE :: No Value!" << "\n";
		B3M = B3MRENDERSTATETYPE::B3MRS_MAX_;
		break;
	}
}

void ChangeToB3MPOOL(B3MPOOL& B3M, FPRHIPOOL FP)
{
	switch (FP)
	{
	case FPRHIPOOL::FPRHIPOOL_SYSTEMMEM:
		B3M = B3MPOOL::B3MPOOL_SYSTEMMEM;
		break;
	default:
		std::cout << "ChangeToB3MPOOL :: No Value!" << "\n";
		B3M = B3MPOOL::B3MPOOL_SYSTEMMEM;
		break;
	}
}

/////////////////////////////////////////////////////////////
//
// MCRHIDevice 내부 구현 클래스 선언
//
class MCRHIDeviceImpl
{
private:
	std::unique_ptr<IMiaDevice9> B3MDevice;
	IMiaVertexBuffer9* MCVertexBuffer = nullptr;

public:
	MCRHIDeviceImpl();
	~MCRHIDeviceImpl();

	std::unique_ptr<IMiaDevice9>* GetDevice() { return &(B3MDevice); }

	////////////////////////////////
	// 인터페이스 연결
public:
	HRESULT BeginScene();
	HRESULT Clear(FPRHICOLOR Color);
	HRESULT DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
	HRESULT EndScene();
	HRESULT Present();

	HRESULT GetDC(HDC* phdc);
	HRESULT ReleaseDC();

	HRESULT SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value);
	HRESULT GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue);


	HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, FPRHIVertexBuffer** ppVertexBuffer, HANDLE* pSharedHandle);
	HRESULT SetStreamSource(UINT StreamNumber, FPRHIVertexBuffer* pStreamData, UINT OffsetInBytes, UINT Stride);
	HRESULT SetFVF(DWORD FVF);

};

/////////////////////////////////////////////////////////////
//
// MCRHIImpl 내부 구현 클래스 선언 
//
class MCRHIImpl
{
private:
	std::unique_ptr<IMia> B3M;

public:
	MCRHIImpl(UINT DeviceVersion);
    ~MCRHIImpl() = default;

    //Com객체 반환
	IMia* GetB3M() { return B3M.get(); }

    //Device 생성
	HRESULT CreateDevice(HWND Hwnd, FPRHIPRESENT_PARAMETERS* Param, DWORD BehaviorFlags, std::unique_ptr<IMiaDevice9>* RHIDevicePointer);
};



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// MCRHIDevice :: 클래스 내부 함수 정의
//
MCRHIDevice::MCRHIDevice() : RHIDeviceImpl(std::make_unique<MCRHIDeviceImpl>())
{
}

MCRHIDevice::~MCRHIDevice()
{
}

HRESULT MCRHIDevice::BeginScene()
{
	return RHIDeviceImpl->BeginScene();
}

HRESULT MCRHIDevice::Clear(DWORD Count, const FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil)
{
	return RHIDeviceImpl->Clear(Color);
}

HRESULT MCRHIDevice::DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	return RHIDeviceImpl->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT MCRHIDevice::EndScene()
{
	return RHIDeviceImpl->EndScene();
}

HRESULT MCRHIDevice::Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	return RHIDeviceImpl->Present();
}

HRESULT MCRHIDevice::GetDC(HDC* phdc)
{
	return RHIDeviceImpl->GetDC(phdc);
}
HRESULT MCRHIDevice::ReleaseDC(HDC hdc)
{
	return RHIDeviceImpl->ReleaseDC();
}

HRESULT MCRHIDevice::SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value)
{
	return RHIDeviceImpl->SetRenderState(State, Value);
}
HRESULT MCRHIDevice::GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue)
{
	return RHIDeviceImpl->GetRenderState(State, pValue);
}

HRESULT MCRHIDevice::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, FPRHIVertexBuffer** ppVertexBuffer, HANDLE* pSharedHandle)
{
	return RHIDeviceImpl->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT MCRHIDevice::SetStreamSource(UINT StreamNumber, FPRHIVertexBuffer* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	return RHIDeviceImpl->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT MCRHIDevice::SetFVF(DWORD FVF)
{
	return RHIDeviceImpl->SetFVF(FVF);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// MCRHI :: 클래스 내부 함수 정의
//
MCRHI::MCRHI(UINT DeviceVersion) : DeviceVersion(DeviceVersion), RHIImpl(std::make_unique<MCRHIImpl>(DeviceVersion))
{
}

MCRHI::~MCRHI()
{
}

HRESULT MCRHI::CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface)
{
	MCRHIDevice* RHIDevice = new MCRHIDevice();

	RHIImpl->CreateDevice(hFocusWindow, pPresentationParameters, BehaviorFlags, RHIDevice->RHIDeviceImpl->GetDevice());

	*ppReturnedDeviceInterface = RHIDevice;

	return E_NOTIMPL;
	// Todo : 매개변수 호출 순서 및 개수 맞추기
}



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// MCRHIDeviceImpl :: 클래스 내부 함수 정의
//
MCRHIDeviceImpl::MCRHIDeviceImpl()
{

}

MCRHIDeviceImpl::~MCRHIDeviceImpl()
{
}

HRESULT MCRHIDeviceImpl::BeginScene()
{
	return B3MDevice->BeginScene();
}

HRESULT MCRHIDeviceImpl::Clear(FPRHICOLOR Color)
{
	return B3MDevice->Clear(Color);
}

HRESULT MCRHIDeviceImpl::DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	B3MPRIMITIVETYPE MCPrimitiveType;
	ChangeToB3MPRIMITIVETYPE(MCPrimitiveType, PrimitiveType);
	return B3MDevice->DrawPrimitive(MCPrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT MCRHIDeviceImpl::EndScene()
{
	return B3MDevice->EndScene();
}

HRESULT MCRHIDeviceImpl::Present()
{
	return B3MDevice->Present();
}


HRESULT MCRHIDeviceImpl::GetDC(HDC* phdc)
{
	*phdc = B3MDevice->GetRT();
	return (phdc != nullptr);
}

HRESULT MCRHIDeviceImpl::ReleaseDC()
{
	return 0;
}


HRESULT MCRHIDeviceImpl::SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value)
{
	B3MRENDERSTATETYPE MCState;
	ChangeToB3MRENDERSTATETYPE(MCState, State);
	return B3MDevice->SetRenderState(MCState, Value);
}

HRESULT MCRHIDeviceImpl::GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue)
{
	B3MRENDERSTATETYPE MCState;
	ChangeToB3MRENDERSTATETYPE(MCState, State);
	return B3MDevice->GetRenderState(MCState, pValue);
}

HRESULT MCRHIDeviceImpl::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, FPRHIVertexBuffer** ppVertexBuffer, HANDLE* pSharedHandle)
{
	B3MPOOL MCPool;
	ChangeToB3MPOOL(MCPool, Pool);
	B3MDevice->CreateVertexBuffer(Length, Usage, FVF, MCPool, &MCVertexBuffer, pSharedHandle);
	return (MCVertexBuffer != nullptr);
}

HRESULT MCRHIDeviceImpl::SetStreamSource(UINT StreamNumber, FPRHIVertexBuffer* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	return B3MDevice->SetStreamSource(StreamNumber, MCVertexBuffer, OffsetInBytes, Stride);
}

HRESULT MCRHIDeviceImpl::SetFVF(DWORD FVF)
{
	return B3MDevice->SetFVF(FVF);
}



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// MCRHIImpl :: 클래스 내부 함수 정의
//
MCRHIImpl::MCRHIImpl(UINT DeviceVersion)
{
	IMia* MP = MiaCreate9(static_cast<DWORD>(DeviceVersion));
	if (MP == nullptr)
	{
		std::cout << "MP == nullptr" << "\n";
	}
	else
	{
		B3M.reset(MP);
	}
}

HRESULT MCRHIImpl::CreateDevice(HWND Hwnd, FPRHIPRESENT_PARAMETERS* Param, DWORD BehaviorFlags, std::unique_ptr<IMiaDevice9>* RHIDevicePointer)
{
	IMiaDevice9* raw = nullptr;

	B3MPRESENT_PARAMETERS MCParam;
	ZeroMemory(&MCParam, sizeof(MCParam));
	ChangeToB3MPRESENT_PARAMETERS(MCParam, Param);

	B3M->CreateDevice(Hwnd, &MCParam, BehaviorFlags, &raw);
	RHIDevicePointer->reset(raw);

	return E_NOTIMPL;
}






