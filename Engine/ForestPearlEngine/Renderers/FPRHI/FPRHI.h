#pragma once
#include "FPRHITypes.h"

//Rendering Hardware Interface
class FPRHI
{
	public :
		virtual HRESULT CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface) = 0;

};

class FPRHIDevice
{
    public:
        virtual HRESULT BeginScene() = 0;
        virtual HRESULT Clear(DWORD Count, CONST FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil) = 0;
        virtual HRESULT EndScene() = 0;
        virtual HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) = 0;
};

//RHI 생성 함수
FPRHI* CreateRHI(UINT DeviceVersion);
