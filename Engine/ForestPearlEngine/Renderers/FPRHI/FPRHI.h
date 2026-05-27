#pragma once

//윈도우 핸들 전방 선언
struct HWND__;
using HWND = HWND__*;

//렌더 타겟 정보 파라미터
typedef struct FPRHIPRESENT_PARAMETERS
{
    unsigned int                BackBufferWidth;    //해상도
    unsigned int                BackBufferHeight;
    int                         BackBufferFormat;
    unsigned int                BackBufferCount;    //백버퍼 개수

    int                         MultiSampleType;
    unsigned long               MultiSampleQuality;

    int                         SwapEffect;
    HWND                        hDeviceWindow;
    int                         Windowed;           //창모드 실행 여부
    int                         EnableAutoDepthStencil;
    int                         AutoDepthStencilFormat;
    unsigned long               Flags;

    /* FullScreen_RefreshRateInHz must be zero for Windowed mode */
    unsigned int                FullScreen_RefreshRateInHz;
    unsigned int                PresentationInterval;

}FPRHIPRESENT_PARAMETERS;

//Rect 정보
typedef struct FPRHIRECT
{
    long left;
    long top;
    long rigth;
    long bottom;

}FPRHIRECT;

//Rendering Hardware Interface
class FPRHI
{
	public :
		virtual int CreateDevice(unsigned int Adapter, int DeviceType, HWND hFocusWindow, unsigned long BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface) = 0; //이걸로 Device를 만들고?

};

class FPRHIDevice
{
    public:
        virtual int BeginScene() = 0; //<- Device에서 호출 하네?
        virtual int Clear(unsigned long  Count, const FPRHIRECT* pRects, unsigned long Flags, unsigned long Color, float Z, unsigned long Stencil) = 0;
        virtual int EndScene() = 0;
        virtual int Present() = 0;
};

//RHI 생성 함수
FPRHI* CreateRHI(int DeviceVersion);
