#pragma once
#include "../../FPRHI/FPRHI.h"
#include <memory>

class DOHWAImpl;
class DOHWADeviceImpl;

class DOHWARHIDevice : public FPRHIDevice
{
private:
    //DOHWADeviceImpl 객체 인터페이스 포인터
    std::unique_ptr<DOHWADeviceImpl> DOHWADeviceimpl;

    //DOHWARHI에서 Device에 접근하기 위해 사용
    friend class DOHWARHI;

public:
    DOHWARHIDevice();
    ~DOHWARHIDevice();
    virtual HRESULT BeginScene() override;
    virtual HRESULT Clear(DWORD Count, CONST FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil) override;
    HRESULT DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) override { return S_OK; };
    virtual HRESULT EndScene() override;
    virtual HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) override;

    virtual HRESULT GetDC(HDC* phdc) override;

    HRESULT SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value) override { return S_OK; };
    HRESULT GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue) override { return S_OK; };


    HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, FPRHIVertexBuffer** ppVertexBuffedr, HANDLE* pSharedHandle) override { return S_OK; };
    HRESULT SetStreamSource(UINT StreamNumber, FPRHIVertexBuffer* pStreamData, UINT OffsetInBytes, UINT Stride) override { return S_OK; };
    HRESULT SetFVF(DWORD FVF) override { return S_OK; };

};

class DOHWARHI : public FPRHI
{
private:
    UINT DeviceVersion;
    // DOHWA 객체 인터페이스 포인터.
    std::unique_ptr<DOHWAImpl> DOHWAimpl;

public:
    DOHWARHI(UINT DeviceVersion);
    ~DOHWARHI();
    virtual HRESULT CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface) override;


};