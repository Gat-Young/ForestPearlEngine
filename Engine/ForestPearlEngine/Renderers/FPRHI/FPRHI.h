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
        virtual HRESULT Clear(DWORD Count, CONST FPRHIRECT* pRects, DWORD Flags, RHICOLOR Color, float Z, DWORD Stencil) = 0;
        virtual HRESULT EndScene() = 0;
        virtual HRESULT Present() = 0;
};

//RHI 생성 함수
FPRHI* CreateRHI(UINT DeviceVersion);
