////////////////////////
// FPRHI에서 사용되는 타입

#pragma once
/////////////////
// 윈도우 환경에서만 동작한다.
#include <windows.h>
#include <mmsystem.h>

//윈도우 핸들 전방 선언
struct HWND__;
using HWND = HWND__*;

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
    LONG x1;
    LONG y1;
    LONG x2;
    LONG y2;

}FPRHIRECT;

// Color 정보
typedef DWORD FPRHICOLOR;

// maps unsigned 8 bits/channel to FPRHICOLOR
#define FPRHICOLOR_ARGB(a,r,g,b) \
    ((FPRHICOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define FPRHICOLOR_RGBA(r,g,b,a) FPRHICOLOR_ARGB(a,r,g,b)
#define FPRHICOLOR_XRGB(r,g,b)   FPRHICOLOR_ARGB(0xff,r,g,b)

#define FPRHICOLOR_XYUV(y,u,v)   FPRHICOLOR_ARGB(0xff,y,u,v)
#define FPRHICOLOR_AYUV(a,y,u,v) FPRHICOLOR_ARGB(a,y,u,v)

// maps floating point channels (0.f to 1.f range) to FPRHICOLOR
#define FPRHICOLOR_COLORVALUE(r,g,b,a) \
    FPRHICOLOR_RGBA((DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f),(DWORD)((a)*255.f))

/* FPRHI Device types */
typedef enum FPRHIDEVTYPE
{
    FPRHIDEVTYPE_HAL = 1,
    FPRHIDEVTYPE_REF = 2,
    FPRHIDEVTYPE_SW = 3,

    FPRHIDEVTYPE_NULLREF = 4,

    FPRHIDEVTYPE_FORCE_DWORD = 0x7fffffff
} FPRHIDEVTYPE;

/* Display Modes */
typedef struct FPRHIDISPLAYMODE
{
    UINT            Width;
    UINT            Height;
    UINT            RefreshRate;
    FPRHIFORMAT     Format;
} FPRHIDISPLAYMODE;

/* Types */
typedef enum FPRHIRESOURCETYPE {
    FPRHIRTYPE_SURFACE = 1,
    FPRHIRTYPE_VOLUME = 2,
    FPRHIRTYPE_TEXTURE = 3,
    FPRHIRTYPE_VOLUMETEXTURE = 4,
    FPRHIRTYPE_CUBETEXTURE = 5,
    FPRHIRTYPE_VERTEXBUFFER = 6,
    FPRHIRTYPE_INDEXBUFFER = 7,           //if this changes, change _D3DDEVINFO_RESOURCEMANAGER definition


    FPRHIRTYPE_FORCE_DWORD = 0x7fffffff
} FPRHIRESOURCETYPE;

/* Pool types */
typedef enum FPRHIPOOL {
    FPRHIPOOL_DEFAULT = 0,
    FPRHIPOOL_MANAGED = 1,
    FPRHIPOOL_SYSTEMMEM = 2,
    FPRHIPOOL_SCRATCH = 3,

    FPRHIPOOL_FORCE_DWORD = 0x7fffffff
} FPRHIPOOL;


//
// PresentationIntervals
//
#define FPRHIPRESENT_INTERVAL_DEFAULT     0x00000000L
#define FPRHIPRESENT_INTERVAL_ONE         0x00000001L
#define FPRHIPRESENT_INTERVAL_TWO         0x00000002L
#define FPRHIPRESENT_INTERVAL_THREE       0x00000004L
#define FPRHIPRESENT_INTERVAL_FOUR        0x00000008L
#define FPRHIPRESENT_INTERVAL_IMMEDIATE   0x80000000L


#define FPRHIADAPTER_DEFAULT                     0

/****************************************************************************
 *
 * Flags for CreateDevice's BehaviorFlags
 *
 ****************************************************************************/

#define FPRHICREATE_FPU_PRESERVE                  0x00000002L
#define FPRHICREATE_MULTITHREADED                 0x00000004L

#define FPRHICREATE_PUREDEVICE                    0x00000010L
#define FPRHICREATE_SOFTWARE_VERTEXPROCESSING     0x00000020L
#define FPRHICREATE_HARDWARE_VERTEXPROCESSING     0x00000040L
#define FPRHICREATE_MIXED_VERTEXPROCESSING        0x00000080L

#define FPRHICREATE_DISABLE_DRIVER_MANAGEMENT     0x00000100L
#define FPRHICREATE_ADAPTERGROUP_DEVICE           0x00000200L
#define FPRHICREATE_DISABLE_DRIVER_MANAGEMENT_EX  0x00000400L

#define FPRHICREATE_NOWINDOWCHANGES				0x00000800L

 /*
  * Options for clearing
  */
#define FPRHICLEAR_TARGET            0x00000001l  /* Clear target surface */
#define FPRHICLEAR_ZBUFFER           0x00000002l  /* Clear target z buffer */
#define FPRHICLEAR_STENCIL           0x00000004l  /* Clear stencil planes */

  /* Usages */
#define FPRHIUSAGE_RENDERTARGET       (0x00000001L)
#define FPRHIUSAGE_DEPTHSTENCIL       (0x00000002L)
#define FPRHIUSAGE_DYNAMIC            (0x00000200L)
#define FPRHIUSAGE_AUTOGENMIPMAP      (0x00000400L)
#define FPRHIUSAGE_DMAP               (0x00004000L)

// The following usages are valid only for querying CheckDeviceFormat
#define FPRHIUSAGE_QUERY_LEGACYBUMPMAP            (0x00008000L)
#define FPRHIUSAGE_QUERY_SRGBREAD                 (0x00010000L)
#define FPRHIUSAGE_QUERY_FILTER                   (0x00020000L)
#define FPRHIUSAGE_QUERY_SRGBWRITE                (0x00040000L)
#define FPRHIUSAGE_QUERY_POSTPIXELSHADER_BLENDING (0x00080000L)
#define FPRHIUSAGE_QUERY_VERTEXTEXTURE            (0x00100000L)
#define FPRHIUSAGE_QUERY_WRAPANDMIP	            (0x00200000L)

/* Usages for Vertex/Index buffers */
#define FPRHIUSAGE_WRITEONLY          (0x00000008L)
#define FPRHIUSAGE_SOFTWAREPROCESSING (0x00000010L)
#define FPRHIUSAGE_DONOTCLIP          (0x00000020L)
#define FPRHIUSAGE_POINTS             (0x00000040L)
#define FPRHIUSAGE_RTPATCHES          (0x00000080L)
#define FPRHIUSAGE_NPATCHES           (0x00000100L)