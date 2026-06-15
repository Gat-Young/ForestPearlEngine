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

typedef enum FPRHITRANSFORMSTATETYPE {
    FPRHITS_VIEW = 2,
    FPRHITS_PROJECTION = 3,
    FPRHITS_TEXTURE0 = 16,
    FPRHITS_TEXTURE1 = 17,
    FPRHITS_TEXTURE2 = 18,
    FPRHITS_TEXTURE3 = 19,
    FPRHITS_TEXTURE4 = 20,
    FPRHITS_TEXTURE5 = 21,
    FPRHITS_TEXTURE6 = 22,
    FPRHITS_TEXTURE7 = 23,
    FPRHITS_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
};

#define FPRHITS_WORLDMATRIX(index) (FPRHITRANSFORMSTATETYPE)(index + 256)
#define FPRHITS_WORLD  FPRHITS_WORLDMATRIX(0)
#define FPRHITS_WORLD1 FPRHITS_WORLDMATRIX(1)
#define FPRHITS_WORLD2 FPRHITS_WORLDMATRIX(2)
#define FPRHITS_WORLD3 FPRHITS_WORLDMATRIX(3)

typedef enum FPRHIRENDERSTATETYPE {
    FPRHIRS_ZENABLE = 7,    /* FPRHIZBUFFERTYPE (or TRUE/FALSE for legacy) */
    FPRHIRS_FILLMODE = 8,    /* FPRHIFILLMODE */
    FPRHIRS_SHADEMODE = 9,    /* FPRHISHADEMODE */
    FPRHIRS_ZWRITEENABLE = 14,   /* TRUE to enable z writes */
    FPRHIRS_ALPHATESTENABLE = 15,   /* TRUE to enable alpha tests */
    FPRHIRS_LASTPIXEL = 16,   /* TRUE for last-pixel on lines */
    FPRHIRS_SRCBLEND = 19,   /* FPRHIBLEND */
    FPRHIRS_DESTBLEND = 20,   /* FPRHIBLEND */
    FPRHIRS_CULLMODE = 22,   /* FPRHICULL */
    FPRHIRS_ZFUNC = 23,   /* FPRHICMPFUNC */
    FPRHIRS_ALPHAREF = 24,   /* FPRHIFIXED */
    FPRHIRS_ALPHAFUNC = 25,   /* FPRHICMPFUNC */
    FPRHIRS_DITHERENABLE = 26,   /* TRUE to enable dithering */
    FPRHIRS_ALPHABLENDENABLE = 27,   /* TRUE to enable alpha blending */
    FPRHIRS_FOGENABLE = 28,   /* TRUE to enable fog blending */
    FPRHIRS_SPECULARENABLE = 29,   /* TRUE to enable specular */
    FPRHIRS_FOGCOLOR = 34,   /* FPRHICOLOR */
    FPRHIRS_FOGTABLEMODE = 35,   /* FPRHIFOGMODE */
    FPRHIRS_FOGSTART = 36,   /* Fog start (for both vertex and pixel fog) */
    FPRHIRS_FOGEND = 37,   /* Fog end      */
    FPRHIRS_FOGDENSITY = 38,   /* Fog density  */
    FPRHIRS_RANGEFOGENABLE = 48,   /* Enables range-based fog */
    FPRHIRS_STENCILENABLE = 52,   /* BOOL enable/disable stenciling */
    FPRHIRS_STENCILFAIL = 53,   /* FPRHISTENCILOP to do if stencil test fails */
    FPRHIRS_STENCILZFAIL = 54,   /* FPRHISTENCILOP to do if stencil test passes and Z test fails */
    FPRHIRS_STENCILPASS = 55,   /* FPRHISTENCILOP to do if both stencil and Z tests pass */
    FPRHIRS_STENCILFUNC = 56,   /* FPRHICMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
    FPRHIRS_STENCILREF = 57,   /* Reference value used in stencil test */
    FPRHIRS_STENCILMASK = 58,   /* Mask value used in stencil test */
    FPRHIRS_STENCILWRITEMASK = 59,   /* Write mask applied to values written to stencil buffer */
    FPRHIRS_TEXTUREFACTOR = 60,   /* FPRHICOLOR used for multi-texture blend */
    FPRHIRS_WRAP0 = 128,  /* wrap for 1st texture coord. set */
    FPRHIRS_WRAP1 = 129,  /* wrap for 2nd texture coord. set */
    FPRHIRS_WRAP2 = 130,  /* wrap for 3rd texture coord. set */
    FPRHIRS_WRAP3 = 131,  /* wrap for 4th texture coord. set */
    FPRHIRS_WRAP4 = 132,  /* wrap for 5th texture coord. set */
    FPRHIRS_WRAP5 = 133,  /* wrap for 6th texture coord. set */
    FPRHIRS_WRAP6 = 134,  /* wrap for 7th texture coord. set */
    FPRHIRS_WRAP7 = 135,  /* wrap for 8th texture coord. set */
    FPRHIRS_CLIPPING = 136,
    FPRHIRS_LIGHTING = 137,
    FPRHIRS_AMBIENT = 139,
    FPRHIRS_FOGVERTEXMODE = 140,
    FPRHIRS_COLORVERTEX = 141,
    FPRHIRS_LOCALVIEWER = 142,
    FPRHIRS_NORMALIZENORMALS = 143,
    FPRHIRS_DIFFUSEMATERIALSOURCE = 145,
    FPRHIRS_SPECULARMATERIALSOURCE = 146,
    FPRHIRS_AMBIENTMATERIALSOURCE = 147,
    FPRHIRS_EMISSIVEMATERIALSOURCE = 148,
    FPRHIRS_VERTEXBLEND = 151,
    FPRHIRS_CLIPPLANEENABLE = 152,
    FPRHIRS_POINTSIZE = 154,   /* float point size */
    FPRHIRS_POINTSIZE_MIN = 155,   /* float point size min threshold */
    FPRHIRS_POINTSPRITEENABLE = 156,   /* BOOL point texture coord control */
    FPRHIRS_POINTSCALEENABLE = 157,   /* BOOL point size scale enable */
    FPRHIRS_POINTSCALE_A = 158,   /* float point attenuation A value */
    FPRHIRS_POINTSCALE_B = 159,   /* float point attenuation B value */
    FPRHIRS_POINTSCALE_C = 160,   /* float point attenuation C value */
    FPRHIRS_MULTISAMPLEANTIALIAS = 161,  // BOOL - set to do FSAA with multisample buffer
    FPRHIRS_MULTISAMPLEMASK = 162,  // DWORD - per-sample enable/disable
    FPRHIRS_PATCHEDGESTYLE = 163,  // Sets whether patch edges will use float style tessellation
    FPRHIRS_DEBUGMONITORTOKEN = 165,  // DEBUG ONLY - token to debug monitor
    FPRHIRS_POINTSIZE_MAX = 166,   /* float point size max threshold */
    FPRHIRS_INDEXEDVERTEXBLENDENABLE = 167,
    FPRHIRS_COLORWRITEENABLE = 168,  // per-channel write enable
    FPRHIRS_TWEENFACTOR = 170,   // float tween factor
    FPRHIRS_BLENDOP = 171,   // FPRHIBLENDOP setting
    FPRHIRS_POSITIONDEGREE = 172,   // NPatch position interpolation degree. FPRHIDEGREE_LINEAR or FPRHIDEGREE_CUBIC (default)
    FPRHIRS_NORMALDEGREE = 173,   // NPatch normal interpolation degree. FPRHIDEGREE_LINEAR (default) or FPRHIDEGREE_QUADRATIC
    FPRHIRS_SCISSORTESTENABLE = 174,
    FPRHIRS_SLOPESCALEDEPTHBIAS = 175,
    FPRHIRS_ANTIALIASEDLINEENABLE = 176,
    FPRHIRS_MINTESSELLATIONLEVEL = 178,
    FPRHIRS_MAXTESSELLATIONLEVEL = 179,
    FPRHIRS_ADAPTIVETESS_X = 180,
    FPRHIRS_ADAPTIVETESS_Y = 181,
    FPRHIRS_ADAPTIVETESS_Z = 182,
    FPRHIRS_ADAPTIVETESS_W = 183,
    FPRHIRS_ENABLEADAPTIVETESSELLATION = 184,
    FPRHIRS_TWOSIDEDSTENCILMODE = 185,   /* BOOL enable/disable 2 sided stenciling */
    FPRHIRS_CCW_STENCILFAIL = 186,   /* FPRHISTENCILOP to do if ccw stencil test fails */
    FPRHIRS_CCW_STENCILZFAIL = 187,   /* FPRHISTENCILOP to do if ccw stencil test passes and Z test fails */
    FPRHIRS_CCW_STENCILPASS = 188,   /* FPRHISTENCILOP to do if both ccw stencil and Z tests pass */
    FPRHIRS_CCW_STENCILFUNC = 189,   /* FPRHICMPFUNC fn.  ccw Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
    FPRHIRS_COLORWRITEENABLE1 = 190,   /* Additional ColorWriteEnables for the devices that support FPRHIPMISCCAPS_INDEPENDENTWRITEMASKS */
    FPRHIRS_COLORWRITEENABLE2 = 191,   /* Additional ColorWriteEnables for the devices that support FPRHIPMISCCAPS_INDEPENDENTWRITEMASKS */
    FPRHIRS_COLORWRITEENABLE3 = 192,   /* Additional ColorWriteEnables for the devices that support FPRHIPMISCCAPS_INDEPENDENTWRITEMASKS */
    FPRHIRS_BLENDFACTOR = 193,   /* FPRHICOLOR used for a constant blend factor during alpha blending for devices that support FPRHIPBLENDCAPS_BLENDFACTOR */
    FPRHIRS_SRGBWRITEENABLE = 194,   /* Enable rendertarget writes to be DE-linearized to SRGB (for formats that expose FPRHIUSAGE_QUERY_SRGBWRITE) */
    FPRHIRS_DEPTHBIAS = 195,
    FPRHIRS_WRAP8 = 198,   /* Additional wrap states for vs_3_0+ attributes with FPRHIDECLUSAGE_TEXCOORD */
    FPRHIRS_WRAP9 = 199,
    FPRHIRS_WRAP10 = 200,
    FPRHIRS_WRAP11 = 201,
    FPRHIRS_WRAP12 = 202,
    FPRHIRS_WRAP13 = 203,
    FPRHIRS_WRAP14 = 204,
    FPRHIRS_WRAP15 = 205,
    FPRHIRS_SEPARATEALPHABLENDENABLE = 206,  /* TRUE to enable a separate blending function for the alpha channel */
    FPRHIRS_SRCBLENDALPHA = 207,  /* SRC blend factor for the alpha channel when FPRHIRS_SEPARATEDESTALPHAENABLE is TRUE */
    FPRHIRS_DESTBLENDALPHA = 208,  /* DST blend factor for the alpha channel when FPRHIRS_SEPARATEDESTALPHAENABLE is TRUE */
    FPRHIRS_BLENDOPALPHA = 209,  /* Blending operation for the alpha channel when FPRHIRS_SEPARATEDESTALPHAENABLE is TRUE */


    FPRHIRS_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
} FPRHIRENDERSTATETYPE;

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
    FPRHIRTYPE_INDEXBUFFER = 7,           //if this changes, change _FPRHIDEVINFO_RESOURCEMANAGER definition


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

// Primitives supported by draw-primitive API
typedef enum FPRHIPRIMITIVETYPE {
    FPRHIPT_POINTLIST = 1,
    FPRHIPT_LINELIST = 2,
    FPRHIPT_LINESTRIP = 3,
    FPRHIPT_TRIANGLELIST = 4,
    FPRHIPT_TRIANGLESTRIP = 5,
    FPRHIPT_TRIANGLEFAN = 6,
    FPRHIPT_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
} FPRHIPRIMITIVETYPE;

typedef enum FPRHICULL {
    FPRHICULL_NONE = 1,
    FPRHICULL_CW = 2,
    FPRHICULL_CCW = 3,
    FPRHICULL_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
} FPRHICULL;

typedef enum FPRHIFILLMODE {
    FPRHIFILL_POINT = 1,
    FPRHIFILL_WIREFRAME = 2,
    FPRHIFILL_SOLID = 3,
    FPRHIFILL_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
} FPRHIFILLMODE;

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

// Flexible vertex format bits
//
#define FPRHIFVF_RESERVED0        0x001
#define FPRHIFVF_POSITION_MASK    0x400E
#define FPRHIFVF_XYZ              0x002
#define FPRHIFVF_XYZRHW           0x004
#define FPRHIFVF_XYZB1            0x006
#define FPRHIFVF_XYZB2            0x008
#define FPRHIFVF_XYZB3            0x00a
#define FPRHIFVF_XYZB4            0x00c
#define FPRHIFVF_XYZB5            0x00e
#define FPRHIFVF_XYZW             0x4002

#define FPRHIFVF_NORMAL           0x010
#define FPRHIFVF_PSIZE            0x020
#define FPRHIFVF_DIFFUSE          0x040
#define FPRHIFVF_SPECULAR         0x080

#define FPRHIFVF_TEXCOUNT_MASK    0xf00
#define FPRHIFVF_TEXCOUNT_SHIFT   8
#define FPRHIFVF_TEX0             0x000
#define FPRHIFVF_TEX1             0x100
#define FPRHIFVF_TEX2             0x200
#define FPRHIFVF_TEX3             0x300
#define FPRHIFVF_TEX4             0x400
#define FPRHIFVF_TEX5             0x500
#define FPRHIFVF_TEX6             0x600
#define FPRHIFVF_TEX7             0x700
#define FPRHIFVF_TEX8             0x800

#define FPRHIFVF_LASTBETA_UBYTE4   0x1000
#define FPRHIFVF_LASTBETA_FPRHICOLOR 0x8000

#define FPRHIFVF_RESERVED2         0x6000  // 2 reserved bits

// Values for FPRHIPRESENT_PARAMETERS.Flags

#define FPRHIPRESENTFLAG_LOCKABLE_BACKBUFFER      0x00000001
#define FPRHIPRESENTFLAG_DISCARD_DEPTHSTENCIL     0x00000002
#define FPRHIPRESENTFLAG_DEVICECLIP               0x00000004
#define FPRHIPRESENTFLAG_VIDEO                    0x00000010


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct FPRHITRANSFORMMATRIX {
    union {
        struct {
            float        position_x, position_y, position_z;
            float        rotation_x, rotation_y, rotation_z;
            float        scale_x,    scale_y,    scale_z;
        };
        float m[3][3];
    };
} FPRHITRANSFORMMATRIX;