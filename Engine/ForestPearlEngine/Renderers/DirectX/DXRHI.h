#pragma once
#include "../FPRHI/FPRHI.h"
#include <memory>


class DXImpl;
class DXDeviceImpl;

class DXRHIDevice : public FPRHIDevice
{
    private:
        //DXDevice 객체 인터페이스 포인터
        std::unique_ptr<DXDeviceImpl> DXDeviceimpl;

        //DXRHI에서 Device에 접근하기 위해 사용
        friend class DXRHI;

    public:
        DXRHIDevice();
        ~DXRHIDevice();
        HRESULT BeginScene() override;
        HRESULT Clear(DWORD Count, CONST FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil) override;
        HRESULT EndScene() override;
        HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) override;
};

class DXRHI : public FPRHI
{
    private :
        UINT DeviceVersion;
        // DX 객체 인터페이스 포인터.
        std::unique_ptr<DXImpl> DXimpl;


	public :
        DXRHI(UINT DeviceVersion);
        ~DXRHI();
        HRESULT CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface) override;
};

