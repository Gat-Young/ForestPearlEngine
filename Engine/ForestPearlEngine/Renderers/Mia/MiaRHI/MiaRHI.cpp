#include "MiaRHI.h"
#include "../Mia/Mia.h"
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
void ChangeB3MPRESENT_PARAMETERS(B3MPRESENT_PARAMETERS& B3M, FPRHIPRESENT_PARAMETERS* pPresentationParameters)
{
	B3M.Width = pPresentationParameters->BackBufferWidth;
	B3M.Height = pPresentationParameters->BackBufferHeight;
	B3M.BackBuffercnt = pPresentationParameters->BackBufferCount;
	B3M.Windowed = pPresentationParameters->Windowed;
}


class MXDeviceImpl
{
private:
	std::unique_ptr<IMiaDevice9> B3MDevice;

public:
	MXDeviceImpl();
	~MXDeviceImpl();

	std::unique_ptr<IMiaDevice9>* GetDevice() { return &(B3MDevice); }
};

class MXImpl
{
private:
	std::unique_ptr<IMia> B3M;

public:
	MXImpl(UINT DeviceVersion);
    ~MXImpl() = default;

    //Com객체 반환
	IMia* GetB3M() { return B3M.get(); }

    //Device 생성
	HRESULT CreateDevice(HWND Hwnd, FPRHIPRESENT_PARAMETERS* Param, DWORD BehaviorFlags, std::unique_ptr<IMiaDevice9>* RHIDevicePointer);
};

MXImpl::MXImpl(UINT DeviceVersion)
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

HRESULT MXImpl::CreateDevice(HWND Hwnd, FPRHIPRESENT_PARAMETERS* Param, DWORD BehaviorFlags, std::unique_ptr<IMiaDevice9>* RHIDevicePointer)
{
	IMiaDevice9* raw = nullptr;

	B3MPRESENT_PARAMETERS MCParam;
	ZeroMemory(&MCParam, sizeof(MCParam));
	ChangeB3MPRESENT_PARAMETERS(MCParam, Param);

	B3M->CreateDevice(Hwnd, &MCParam, BehaviorFlags, &raw);
	RHIDevicePointer->reset(raw);

	return E_NOTIMPL;
}

MCRHIDevice::MCRHIDevice() : MXDeviceimpl(std::make_unique<MXDeviceImpl>())
{
}

MCRHIDevice::~MCRHIDevice()
{
}

HRESULT MCRHIDevice::BeginScene()
{
	return E_NOTIMPL;
}

HRESULT MCRHIDevice::Clear(DWORD Count, const FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil)
{
	return E_NOTIMPL;
}

HRESULT MCRHIDevice::EndScene()
{
	return E_NOTIMPL;
}

HRESULT MCRHIDevice::Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	return E_NOTIMPL;
}

MCRHI::MCRHI(UINT DeviceVersion) : DeviceVersion(DeviceVersion), MXimpl(std::make_unique<MXImpl>(DeviceVersion))
{
}

MCRHI::~MCRHI()
{
}

HRESULT MCRHI::CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface)
{
	MCRHIDevice* RHIDevice = new MCRHIDevice();

	MXimpl->CreateDevice(hFocusWindow, pPresentationParameters, BehaviorFlags, RHIDevice->MXDeviceimpl->GetDevice());

	*ppReturnedDeviceInterface = RHIDevice;

	return E_NOTIMPL;
	// Todo : 매개변수 호출 순서 및 개수 맞추기
}

MXDeviceImpl::MXDeviceImpl() :
{

}

MXDeviceImpl::~MXDeviceImpl()
{
}
