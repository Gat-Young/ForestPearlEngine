#pragma once
#include "../../FPRHI/FPRHI.h"
#include <memory>

class MXImpl;
class MXDeviceImpl;

class MCRHIDevice : public FPRHIDevice
{
private:
    //DXDevice 객체 인터페이스 포인터
    std::unique_ptr<MXDeviceImpl> MXDeviceimpl;

    //DXRHI에서 Device에 접근하기 위해 사용
    friend class MCRHI;

    public:
        MCRHIDevice();
        ~MCRHIDevice();
        virtual HRESULT BeginScene() override;
        virtual HRESULT Clear(DWORD Count, CONST FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil) override;
        virtual HRESULT EndScene() override;
        virtual HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) override;

};

class MCRHI : public FPRHI
{
private:
    UINT DeviceVersion;
    // MX 객체 인터페이스 포인터.
    std::unique_ptr<MXImpl> MXimpl;

public:
    MCRHI(UINT DeviceVersion);
    ~MCRHI();
    virtual HRESULT CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface) override;


};