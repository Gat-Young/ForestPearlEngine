#pragma once
#include "../FPRHI/FPRHI.h"
#include <memory>


class DXImpl;
class DXDeviceImpl;
class DXVertexBufferImpl;

class DXRHIVertexBuffer : public FPRHIVertexBuffer
{
    private :
        //DXVertexBuffer 객체 인터페이스 포인터
        std::unique_ptr<DXVertexBufferImpl> DXVertexBufferimpl;

        //DXRHIDevice에서 VertexBuffer에 접근하기 위해 사용
        friend class DXRHIDevice;

    public :
        DXRHIVertexBuffer();
        ~DXRHIVertexBuffer();
        HRESULT Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags) override;
        HRESULT Unlock() override;
};

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
        HRESULT DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) override;
        HRESULT EndScene() override;
        HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) override;

        HRESULT GetDC(HDC* phdc) override;
        HRESULT ReleaseDC(HDC hdc) override;

        HRESULT SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value) override;
        HRESULT GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue) override;


        HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, FPRHIVertexBuffer** ppVertexBuffedr, HANDLE* pSharedHandle) override;
        HRESULT SetStreamSource(UINT StreamNumber, FPRHIVertexBuffer* pStreamData, UINT OffsetInBytes, UINT Stride) override;
        HRESULT SetFVF(DWORD FVF) override;
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

