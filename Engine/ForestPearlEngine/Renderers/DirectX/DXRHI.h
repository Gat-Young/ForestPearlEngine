#pragma once
#include "../FPRHI/FPRHI.h"
#include <memory>

class DXRHI : public FPRHI
{
    private :
        UINT DeviceVersion;
        class DXImpl;
        // DX 객체 인터페이스 포인터.
        std::unique_ptr<DXImpl> DXimpl;


	public :
        DXRHI(UINT DeviceVersion);
        ~DXRHI();
        HRESULT CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface) override;
};

class DXRHIDevice : public FPRHIDevice
{
    private:
        class DXDeviceImpl;
        //DXDevice 객체 인터페이스 포인터
        std::unique_ptr<DXDeviceImpl> DXDeviceimpl;

        //DXRHI에서 Device에 접근하기 위해 사용
        friend class DXRHI;

    public:
        DXRHIDevice();
        ~DXRHIDevice();
        HRESULT BeginScene() override;
        HRESULT Clear(DWORD Count, CONST FPRHIRECT* pRects, DWORD Flags, RHICOLOR Color, float Z, DWORD Stencil) override;
        HRESULT EndScene() override;
        HRESULT Present() override;
};

