#pragma once
#include "FPRHITypes.h"
#include <stack>

//Vertex Buffer Interface
class FPRHIVertexBuffer
{
    public:
        virtual HRESULT Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags) = 0;
        virtual HRESULT Unlock() = 0;

};

//Rendering Device Interface
class FPRHIDevice
{
    public:
        virtual HRESULT BeginScene() = 0;
        virtual HRESULT Clear(DWORD Count, CONST FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil) = 0;
        virtual HRESULT DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) = 0;
        virtual HRESULT EndScene() = 0;
        virtual HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) = 0;

        virtual HRESULT GetDC(HDC* phdc) = 0;
        virtual HRESULT ReleaseDC(HDC hdc) = 0;

        virtual HRESULT SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value) = 0;
        virtual HRESULT GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue) = 0;


        virtual HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, FPRHIVertexBuffer** ppVertexBuffedr, HANDLE* pSharedHandle) = 0;
        virtual HRESULT SetStreamSource(UINT StreamNumber, FPRHIVertexBuffer* pStreamData, UINT OffsetInBytes, UINT Stride) = 0;
        virtual HRESULT SetFVF(DWORD FVF) = 0;

        //나중에 꼭 수정
        virtual HRESULT SetTransform(FPRHITRANSFORMSTATETYPE State, FPRHITRANSFORMMATRIX* pMatrix) = 0;

};

//Rendering Hardware Interface
class FPRHI
{
	public :
		virtual HRESULT CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface) = 0;

};

//RHI 생성 함수
FPRHI* CreateRHI(UINT DeviceVersion);
