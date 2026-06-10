#pragma once
#include "../FPRHI/FPRHI.h"
#include <memory>

class MCRHIImpl;
class MCRHIDeviceImpl;

class MCRHIDevice : public FPRHIDevice
{
private:
    //DXDevice 객체 인터페이스 포인터
    std::unique_ptr<MCRHIDeviceImpl> RHIDeviceImpl;

    //DXRHI에서 Device에 접근하기 위해 사용
    friend class MCRHI;

    public:
        MCRHIDevice();
        ~MCRHIDevice();

    ////////////////////////////////
    // 인터페이스 override
    public:
        virtual HRESULT BeginScene() override;
        virtual HRESULT Clear(DWORD Count, CONST FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil) override;
        virtual HRESULT DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) override;
        virtual HRESULT EndScene() override;
        virtual HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) override;

        virtual HRESULT GetDC(HDC* phdc) override;
        virtual HRESULT ReleaseDC(HDC hdc) override;

        virtual HRESULT SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value) override;
        virtual HRESULT GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue) override;


        virtual HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, FPRHIVertexBuffer** ppVertexBuffer, HANDLE* pSharedHandle) override;
        virtual HRESULT SetStreamSource(UINT StreamNumber, FPRHIVertexBuffer* pStreamData, UINT OffsetInBytes, UINT Stride) override;
        virtual HRESULT SetFVF(DWORD FVF) override;
};

class MCRHI : public FPRHI
{
private:
    UINT DeviceVersion;
    // MX 객체 인터페이스 포인터.
    std::unique_ptr<MCRHIImpl> RHIImpl;

public:
    MCRHI(UINT DeviceVersion);
    ~MCRHI();
    virtual HRESULT CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface) override;


};