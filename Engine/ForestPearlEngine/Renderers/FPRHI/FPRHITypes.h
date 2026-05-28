////////////////////////
// FPRHI에서 사용되는 타입

#pragma once
/////////////////
// 윈도우 환경에서만 동작한다.
#include<minwindef.h>   //Windows API를 쓰기 위한 최소한의 기본 정의
#include<mmsyscom.h>    //Windows 멀티미디어 시스템 공통 정의용 헤더

//윈도우 핸들 전방 선언
struct HWND__;
using HWND = HWND__*;

//렌더 타겟 정보 파라미터
typedef struct FPRHIPRESENT_PARAMETERS
{
    UINT                        BackBufferWidth;
    UINT                        BackBufferHeight;
    FPRHIFORMAT                 BackBufferFormat;
    UINT                        BackBufferCount;        //백버퍼 개수

    FPRHIMULTISAMPLE_TYPE       MultiSampleType;
    DWORD                       MultiSampleQuality;

    FPRHISWAPEFFECT             SwapEffect;
    HWND                        hDeviceWindow;
    BOOL                        Windowed;               //창모드 실행 여부
    BOOL                        EnableAutoDepthStencil;
    FPRHIFORMAT                 AutoDepthStencilFormat;
    DWORD                       Flags;

    /* FullScreen_RefreshRateInHz must be zero for Windowed mode */
    UINT                FullScreen_RefreshRateInHz;
    UINT                PresentationInterval;

}FPRHIPRESENT_PARAMETERS;

//Rect 정보
typedef struct FPRHIRECT
{
    LONG left;
    LONG top;
    LONG rigth;
    LONG bottom;

}FPRHIRECT;

// Color 정보
typedef DWORD RHICOLOR;

// Format 정보
typedef enum FPRHIFORMAT
{
    FPRHIFMT_UNKNOWN = 0,

    FPRHIFMT_R8G8B8 = 20,
    FPRHIFMT_A8R8G8B8 = 21,
    FPRHIFMT_X8R8G8B8 = 22,
    FPRHIFMT_R5G6B5 = 23,
    FPRHIFMT_X1R5G5B5 = 24,
    FPRHIFMT_A1R5G5B5 = 25,
    FPRHIFMT_A4R4G4B4 = 26,
    FPRHIFMT_R3G3B2 = 27,
    FPRHIFMT_A8 = 28,
    FPRHIFMT_A8R3G3B2 = 29,
    FPRHIFMT_X4R4G4B4 = 30,
    FPRHIFMT_A2B10G10R10 = 31,
    FPRHIFMT_A8B8G8R8 = 32,
    FPRHIFMT_X8B8G8R8 = 33,
    FPRHIFMT_G16R16 = 34,
    FPRHIFMT_A2R10G10B10 = 35,
    FPRHIFMT_A16B16G16R16 = 36,

    FPRHIFMT_A8P8 = 40,
    FPRHIFMT_P8 = 41,

    FPRHIFMT_L8 = 50,
    FPRHIFMT_A8L8 = 51,
    FPRHIFMT_A4L4 = 52,

    FPRHIFMT_V8U8 = 60,
    FPRHIFMT_L6V5U5 = 61,
    FPRHIFMT_X8L8V8U8 = 62,
    FPRHIFMT_Q8W8V8U8 = 63,
    FPRHIFMT_V16U16 = 64,
    FPRHIFMT_A2W10V10U10 = 67,

    FPRHIFMT_UYVY = MAKEFOURCC('U', 'Y', 'V', 'Y'),
    FPRHIFMT_R8G8_B8G8 = MAKEFOURCC('R', 'G', 'B', 'G'),
    FPRHIFMT_YUY2 = MAKEFOURCC('Y', 'U', 'Y', '2'),
    FPRHIFMT_G8R8_G8B8 = MAKEFOURCC('G', 'R', 'G', 'B'),
    FPRHIFMT_DXT1 = MAKEFOURCC('D', 'X', 'T', '1'),
    FPRHIFMT_DXT2 = MAKEFOURCC('D', 'X', 'T', '2'),
    FPRHIFMT_DXT3 = MAKEFOURCC('D', 'X', 'T', '3'),
    FPRHIFMT_DXT4 = MAKEFOURCC('D', 'X', 'T', '4'),
    FPRHIFMT_DXT5 = MAKEFOURCC('D', 'X', 'T', '5'),

    FPRHIFMT_D16_LOCKABLE = 70,
    FPRHIFMT_D32 = 71,
    FPRHIFMT_D15S1 = 73,
    FPRHIFMT_D24S8 = 75,
    FPRHIFMT_D24X8 = 77,
    FPRHIFMT_D24X4S4 = 79,
    FPRHIFMT_D16 = 80,

    FPRHIFMT_D32F_LOCKABLE = 82,
    FPRHIFMT_D24FS8 = 83,

    FPRHIFMT_L16 = 81,

    FPRHIFMT_VERTEXDATA = 100,
    FPRHIFMT_INDEX16 = 101,
    FPRHIFMT_INDEX32 = 102,

    FPRHIFMT_Q16W16V16U16 = 110,

    FPRHIFMT_MULTI2_ARGB8 = MAKEFOURCC('M', 'E', 'T', '1'),

    // Floating point surface formats

    // s10e5 formats (16-bits per channel)
    FPRHIFMT_R16F = 111,
    FPRHIFMT_G16R16F = 112,
    FPRHIFMT_A16B16G16R16F = 113,

    // IEEE s23e8 formats (32-bits per channel)
    FPRHIFMT_R32F = 114,
    FPRHIFMT_G32R32F = 115,
    FPRHIFMT_A32B32G32R32F = 116,

    FPRHIFMT_CxV8U8 = 117,

    FPRHIFMT_FORCE_DWORD = 0x7fffffff
} FPRHIFORMAT;

/* FPRHI Multi-Sample buffer types */
typedef enum FPRHIMULTISAMPLE_TYPE
{
    FPRHIMULTISAMPLE_NONE = 0,
    FPRHIMULTISAMPLE_NONMASKABLE = 1,
    FPRHIMULTISAMPLE_2_SAMPLES = 2,
    FPRHIMULTISAMPLE_3_SAMPLES = 3,
    FPRHIMULTISAMPLE_4_SAMPLES = 4,
    FPRHIMULTISAMPLE_5_SAMPLES = 5,
    FPRHIMULTISAMPLE_6_SAMPLES = 6,
    FPRHIMULTISAMPLE_7_SAMPLES = 7,
    FPRHIMULTISAMPLE_8_SAMPLES = 8,
    FPRHIMULTISAMPLE_9_SAMPLES = 9,
    FPRHIMULTISAMPLE_10_SAMPLES = 10,
    FPRHIMULTISAMPLE_11_SAMPLES = 11,
    FPRHIMULTISAMPLE_12_SAMPLES = 12,
    FPRHIMULTISAMPLE_13_SAMPLES = 13,
    FPRHIMULTISAMPLE_14_SAMPLES = 14,
    FPRHIMULTISAMPLE_15_SAMPLES = 15,
    FPRHIMULTISAMPLE_16_SAMPLES = 16,

    FPRHIMULTISAMPLE_FORCE_DWORD = 0x7fffffff
} FPRHIMULTISAMPLE_TYPE;

/* FPRHISwapEffects */
typedef enum FPRHISWAPEFFECT
{
    FPRHISWAPEFFECT_DISCARD = 1,
    FPRHISWAPEFFECT_FLIP = 2,
    FPRHISWAPEFFECT_COPY = 3,

    FPRHISWAPEFFECT_FORCE_DWORD = 0x7fffffff
} FPRHISWAPEFFECT;

/* FPRHI Device types */
typedef enum FPRHIDEVTYPE
{
    FPRHIDEVTYPE_HAL = 1,
    FPRHIDEVTYPE_REF = 2,
    FPRHIDEVTYPE_SW = 3,

    FPRHIDEVTYPE_NULLREF = 4,

    FPRHIDEVTYPE_FORCE_DWORD = 0x7fffffff
} FPRHIDEVTYPE;

