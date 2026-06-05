#include "DXRHI.h"
#include <d3d9.h>
#include <wrl/client.h>
#include <iostream>

#pragma comment(lib, "d3d9.lib")

template<typename T>
using Comptr = Microsoft::WRL::ComPtr<T>;


/////////////////////////
// 전역 함수
// 생성 함수
FPRHI* CreateRHI(UINT DeviceVersion)
{
    return new DXRHI(DeviceVersion);
}

/////////////////////////
// 전역 함수
// 타입 변환기
//FPRHIDEVTYPE To D3DDEVTYPE 타입 변환기
D3DDEVTYPE ChangeD3DDEVTYPE(FPRHIDEVTYPE DeviceType)
{
    switch (DeviceType)
    {
        case FPRHIDEVTYPE_HAL: return D3DDEVTYPE_HAL;
        case FPRHIDEVTYPE_REF:	return D3DDEVTYPE_REF;
        case FPRHIDEVTYPE_SW:	return D3DDEVTYPE_SW;
        case FPRHIDEVTYPE_NULLREF:	return D3DDEVTYPE_NULLREF;
        case FPRHIDEVTYPE_FORCE_DWORD:	return D3DDEVTYPE_FORCE_DWORD;
    }
}

//FPRHIFORMAT To D3DFORMAT 타입 변환기
D3DFORMAT ChangeD3DFORMAT(FPRHIFORMAT FPRHIFormat)
{
    switch (FPRHIFormat)
    {
        case FPRHIFMT_UNKNOWN:   return D3DFMT_UNKNOWN;

        case FPRHIFMT_R8G8B8:   return D3DFMT_R8G8B8;
        case FPRHIFMT_A8R8G8B8:   return D3DFMT_A8R8G8B8;
        case FPRHIFMT_X8R8G8B8:   return D3DFMT_X8R8G8B8;
        case FPRHIFMT_R5G6B5:   return D3DFMT_R5G6B5;
        case FPRHIFMT_X1R5G5B5:
        case FPRHIFMT_A1R5G5B5:   return D3DFMT_A1R5G5B5;
        case FPRHIFMT_A4R4G4B4:   return D3DFMT_A4R4G4B4;
        case FPRHIFMT_R3G3B2:   return D3DFMT_R3G3B2;
        case FPRHIFMT_A8:   return D3DFMT_A8;
        case FPRHIFMT_A8R3G3B2:   return D3DFMT_A8R3G3B2;
        case FPRHIFMT_X4R4G4B4:   return D3DFMT_X4R4G4B4;
        case FPRHIFMT_A2B10G10R10:   return D3DFMT_A2B10G10R10;
        case FPRHIFMT_A8B8G8R8:   return D3DFMT_A8B8G8R8;
        case FPRHIFMT_X8B8G8R8:   return D3DFMT_X8B8G8R8;
        case FPRHIFMT_G16R16:   return D3DFMT_G16R16;
        case FPRHIFMT_A2R10G10B10:   return D3DFMT_A2R10G10B10;
        case FPRHIFMT_A16B16G16R16:   return D3DFMT_A16B16G16R16;

        case FPRHIFMT_A8P8:   return D3DFMT_A8P8;
        case FPRHIFMT_P8:   return D3DFMT_P8;

        case FPRHIFMT_L8:   return D3DFMT_L8;
        case FPRHIFMT_A8L8:   return D3DFMT_A8L8;
        case FPRHIFMT_A4L4:   return D3DFMT_A4L4;

        case FPRHIFMT_V8U8:   return D3DFMT_V8U8;
        case FPRHIFMT_L6V5U5:   return D3DFMT_L6V5U5;
        case FPRHIFMT_X8L8V8U8:   return D3DFMT_X8L8V8U8;
        case FPRHIFMT_Q8W8V8U8:   return D3DFMT_Q8W8V8U8;
        case FPRHIFMT_V16U16:   return D3DFMT_V16U16;
        case FPRHIFMT_A2W10V10U10:   return D3DFMT_A2W10V10U10;

        case FPRHIFMT_UYVY:   return D3DFMT_UYVY;
        case FPRHIFMT_R8G8_B8G8:   return D3DFMT_R8G8_B8G8;
        case FPRHIFMT_YUY2:   return D3DFMT_YUY2;
        case FPRHIFMT_G8R8_G8B8:   return D3DFMT_G8R8_G8B8;
        case FPRHIFMT_DXT1:   return D3DFMT_DXT1;
        case FPRHIFMT_DXT2:   return D3DFMT_DXT2;
        case FPRHIFMT_DXT3:   return D3DFMT_DXT3;
        case FPRHIFMT_DXT4:   return D3DFMT_DXT4;
        case FPRHIFMT_DXT5:   return D3DFMT_DXT5;

        case FPRHIFMT_D16_LOCKABLE:   return D3DFMT_D16_LOCKABLE;
        case FPRHIFMT_D32:   return D3DFMT_D32;
        case FPRHIFMT_D15S1:   return D3DFMT_D15S1;
        case FPRHIFMT_D24S8:   return D3DFMT_D24S8;
        case FPRHIFMT_D24X8:   return D3DFMT_D24X8;
        case FPRHIFMT_D24X4S4:   return D3DFMT_D24X4S4;
        case FPRHIFMT_D16:   return D3DFMT_D16;

        case FPRHIFMT_D32F_LOCKABLE:   return D3DFMT_D32F_LOCKABLE;
        case FPRHIFMT_D24FS8:   return D3DFMT_D24FS8;

        case FPRHIFMT_L16:   return D3DFMT_L16;

        case FPRHIFMT_VERTEXDATA:   return D3DFMT_VERTEXDATA;
        case FPRHIFMT_INDEX16:   return D3DFMT_INDEX16;
        case FPRHIFMT_INDEX32:   return D3DFMT_INDEX32;

        case FPRHIFMT_Q16W16V16U16:   return D3DFMT_Q16W16V16U16;

        case FPRHIFMT_MULTI2_ARGB8:   return D3DFMT_MULTI2_ARGB8;

            // Floating point surface formats

            // s10e5 formats (16-bits per channel)
        case FPRHIFMT_R16F:   return D3DFMT_R16F;
        case FPRHIFMT_G16R16F:   return D3DFMT_G16R16F;
        case FPRHIFMT_A16B16G16R16F:   return D3DFMT_A16B16G16R16F;

            // IEEE s23e8 formats (32-bits per channel)
        case FPRHIFMT_R32F:   return D3DFMT_R32F;
        case FPRHIFMT_G32R32F:   return D3DFMT_G32R32F;
        case FPRHIFMT_A32B32G32R32F:   return D3DFMT_A32B32G32R32F;

        case FPRHIFMT_CxV8U8:   return D3DFMT_CxV8U8;

        case FPRHIFMT_FORCE_DWORD:   return D3DFMT_FORCE_DWORD;
    }
}

//FPRHIMULTISAMPLE_TYPE To D3DMULTISAMPLE_TYPE 타입 변환기
D3DMULTISAMPLE_TYPE ChangeD3DFORMAT(FPRHIMULTISAMPLE_TYPE FPRHIMultisampletype)
{
    switch (FPRHIMultisampletype)
    {
        case FPRHIMULTISAMPLE_NONE:   return D3DMULTISAMPLE_NONE;
        case FPRHIMULTISAMPLE_NONMASKABLE:   return D3DMULTISAMPLE_NONMASKABLE;
        case FPRHIMULTISAMPLE_2_SAMPLES:   return D3DMULTISAMPLE_2_SAMPLES;
        case FPRHIMULTISAMPLE_3_SAMPLES:   return D3DMULTISAMPLE_3_SAMPLES;
        case FPRHIMULTISAMPLE_4_SAMPLES:   return D3DMULTISAMPLE_4_SAMPLES;
        case FPRHIMULTISAMPLE_5_SAMPLES:   return D3DMULTISAMPLE_5_SAMPLES;
        case FPRHIMULTISAMPLE_6_SAMPLES:   return D3DMULTISAMPLE_6_SAMPLES;
        case FPRHIMULTISAMPLE_7_SAMPLES:   return D3DMULTISAMPLE_7_SAMPLES;
        case FPRHIMULTISAMPLE_8_SAMPLES:   return D3DMULTISAMPLE_8_SAMPLES;
        case FPRHIMULTISAMPLE_9_SAMPLES:   return D3DMULTISAMPLE_9_SAMPLES;
        case FPRHIMULTISAMPLE_10_SAMPLES:   return D3DMULTISAMPLE_10_SAMPLES;
        case FPRHIMULTISAMPLE_11_SAMPLES:   return D3DMULTISAMPLE_11_SAMPLES;
        case FPRHIMULTISAMPLE_12_SAMPLES:   return D3DMULTISAMPLE_12_SAMPLES;
        case FPRHIMULTISAMPLE_13_SAMPLES:   return D3DMULTISAMPLE_13_SAMPLES;
        case FPRHIMULTISAMPLE_14_SAMPLES:   return D3DMULTISAMPLE_14_SAMPLES;
        case FPRHIMULTISAMPLE_15_SAMPLES:   return D3DMULTISAMPLE_15_SAMPLES;
        case FPRHIMULTISAMPLE_16_SAMPLES:   return D3DMULTISAMPLE_16_SAMPLES;

        case FPRHIMULTISAMPLE_FORCE_DWORD:   return D3DMULTISAMPLE_FORCE_DWORD;
    }
}

//FPRHISWAPEFFECT To D3DSWAPEFFECT 타입 변환기
D3DSWAPEFFECT ChangeD3DSWAPEFFECT(FPRHISWAPEFFECT FPRHISwapeffect)
{
    switch (FPRHISwapeffect)
    {
        case FPRHISWAPEFFECT_DISCARD:   return D3DSWAPEFFECT_DISCARD;
        case FPRHISWAPEFFECT_FLIP:   return D3DSWAPEFFECT_FLIP;
        case FPRHISWAPEFFECT_COPY:   return D3DSWAPEFFECT_COPY;

        case FPRHISWAPEFFECT_FORCE_DWORD:   return D3DSWAPEFFECT_FORCE_DWORD;
    }
}

//FPRHIMULTISAMPLE_TYPE To D3DMULTISAMPLE_TYPE 타입 변환기
D3DMULTISAMPLE_TYPE ChangeD3DMULTISAMPLE_TYPE(FPRHIMULTISAMPLE_TYPE FPRHIMultiSmapleType)
{
    switch (FPRHIMultiSmapleType)
    {
        case FPRHIMULTISAMPLE_NONE: return D3DMULTISAMPLE_NONE;
        case FPRHIMULTISAMPLE_NONMASKABLE: return D3DMULTISAMPLE_NONMASKABLE;
        case FPRHIMULTISAMPLE_2_SAMPLES: return D3DMULTISAMPLE_2_SAMPLES;
        case FPRHIMULTISAMPLE_3_SAMPLES: return D3DMULTISAMPLE_3_SAMPLES;
        case FPRHIMULTISAMPLE_4_SAMPLES: return D3DMULTISAMPLE_4_SAMPLES;
        case FPRHIMULTISAMPLE_5_SAMPLES: return D3DMULTISAMPLE_5_SAMPLES;
        case FPRHIMULTISAMPLE_6_SAMPLES: return D3DMULTISAMPLE_6_SAMPLES;
        case FPRHIMULTISAMPLE_7_SAMPLES: return D3DMULTISAMPLE_7_SAMPLES;
        case FPRHIMULTISAMPLE_8_SAMPLES: return D3DMULTISAMPLE_8_SAMPLES;
        case FPRHIMULTISAMPLE_9_SAMPLES: return D3DMULTISAMPLE_9_SAMPLES;
        case FPRHIMULTISAMPLE_10_SAMPLES: return D3DMULTISAMPLE_10_SAMPLES;
        case FPRHIMULTISAMPLE_11_SAMPLES: return D3DMULTISAMPLE_11_SAMPLES;
        case FPRHIMULTISAMPLE_12_SAMPLES: return D3DMULTISAMPLE_12_SAMPLES;
        case FPRHIMULTISAMPLE_13_SAMPLES: return D3DMULTISAMPLE_13_SAMPLES;
        case FPRHIMULTISAMPLE_14_SAMPLES: return D3DMULTISAMPLE_14_SAMPLES;
        case FPRHIMULTISAMPLE_15_SAMPLES: return D3DMULTISAMPLE_15_SAMPLES;
        case FPRHIMULTISAMPLE_16_SAMPLES: return D3DMULTISAMPLE_16_SAMPLES;

        case FPRHIMULTISAMPLE_FORCE_DWORD: return D3DMULTISAMPLE_FORCE_DWORD;
    }
}

//FPRHIRECT To D3DRECT 타입 변환기
void ChangeD3DRECT(const FPRHIRECT* FPRHIRect, D3DRECT& D3DRect)
{
    if (FPRHIRect == NULL)
    {
        return;
    }

    D3DRect.x1 = FPRHIRect->x1;
    D3DRect.y1 = FPRHIRect->y1;
    D3DRect.x2 = FPRHIRect->x2;
    D3DRect.y2 = FPRHIRect->y2;
}

//FPRHICOLOR To D3DCOLOR 타입 변환기
void ChangeD3DCOLOR(FPRHICOLOR& FPRHIColor, D3DCOLOR& D3DColor)
{
    D3DColor = FPRHIColor;
}

//FPRHIPARAMETERSFLAG To D3DPARAMETERSFLAG 타입 변환기
DWORD ChangeD3DPARAMETERSFLAG(DWORD FPRHIParameterFlag)
{
    switch (FPRHIParameterFlag)
    {
    case FPRHIPRESENTFLAG_LOCKABLE_BACKBUFFER:           return D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    case FPRHIPRESENTFLAG_DISCARD_DEPTHSTENCIL:       return D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    case FPRHIPRESENTFLAG_DEVICECLIP:           return D3DPRESENTFLAG_DEVICECLIP;
    case FPRHIPRESENTFLAG_VIDEO:     return D3DPRESENTFLAG_VIDEO;

    default:                        return NULL;
    }
}

//FPRHIPRESENT_PARAMETERS To D3DPRESENT_PARAMETERS 타입 변환기
void ChangeD3DPRESENT_PARAMETERS(D3DPRESENT_PARAMETERS& D3D, FPRHIPRESENT_PARAMETERS* pPresentationParameters)
{
    D3D.BackBufferWidth = pPresentationParameters->BackBufferWidth;
    D3D.BackBufferHeight = pPresentationParameters->BackBufferHeight;
    D3D.BackBufferFormat = ChangeD3DFORMAT(pPresentationParameters->BackBufferFormat);
    D3D.BackBufferCount = pPresentationParameters->BackBufferCount;

    D3D.MultiSampleType = ChangeD3DMULTISAMPLE_TYPE(pPresentationParameters->MultiSampleType);
    D3D.MultiSampleQuality = pPresentationParameters->MultiSampleQuality;

    D3D.SwapEffect = ChangeD3DSWAPEFFECT(pPresentationParameters->SwapEffect);
    D3D.hDeviceWindow = pPresentationParameters->hDeviceWindow;
    D3D.Windowed = pPresentationParameters->Windowed;
    D3D.EnableAutoDepthStencil = pPresentationParameters->EnableAutoDepthStencil;
    D3D.AutoDepthStencilFormat = ChangeD3DFORMAT(pPresentationParameters->AutoDepthStencilFormat);
    D3D.Flags = ChangeD3DPARAMETERSFLAG(pPresentationParameters->Flags);

    D3D.FullScreen_RefreshRateInHz = pPresentationParameters->FullScreen_RefreshRateInHz;
    D3D.PresentationInterval = pPresentationParameters->PresentationInterval;

}

//FPRHIFVF To D3DFVF 타입 변환기
DWORD ChangeD3DFVF_FORMAT(DWORD FPRHIFvf)
{

    //위치 타입만 뽑아내기
    DWORD FPRHIPosType = FPRHIFvf & FPRHIFVF_POSITION_MASK;

    DWORD D3DPosType;

    switch (FPRHIPosType)
    {
        case  FPRHIFVF_XYZ:       D3DPosType = D3DFVF_XYZ; break;
        case  FPRHIFVF_XYZRHW:    D3DPosType = D3DFVF_XYZRHW; break;
        case  FPRHIFVF_XYZB1:     D3DPosType = D3DFVF_XYZB1; break;
        case  FPRHIFVF_XYZB2:     D3DPosType = D3DFVF_XYZB2; break;
        case  FPRHIFVF_XYZB3:     D3DPosType = D3DFVF_XYZB3; break;
        case  FPRHIFVF_XYZB4:     D3DPosType = D3DFVF_XYZB4; break;
        case  FPRHIFVF_XYZB5:     D3DPosType = D3DFVF_XYZB5; break;
        case  FPRHIFVF_XYZW:      D3DPosType = D3DFVF_XYZW; break;
        default:                  D3DPosType = 0x000; break;
    }

    //법선 벡터 여부
    DWORD DXNormal = ((FPRHIFvf & FPRHIFVF_NORMAL) == D3DFVF_NORMAL) ? D3DFVF_NORMAL : 0x000;

    // 점 프리미티브 크기 여부
    DWORD DXPsize = ((FPRHIFvf & FPRHIFVF_PSIZE) == D3DFVF_PSIZE) ? D3DFVF_PSIZE : 0x000;

    //Diffuse Color 여부
    DWORD DXDiffuse = ((FPRHIFvf & FPRHIFVF_DIFFUSE) == D3DFVF_DIFFUSE) ? D3DFVF_DIFFUSE : 0x000;

    //Specular Color 여부
    DWORD DXSpecular = ((FPRHIFvf & FPRHIFVF_SPECULAR) == D3DFVF_SPECULAR) ? D3DFVF_SPECULAR : 0x000;


    //텍스처 좌표 개수 뽑아내기
    DWORD FPRHITextCount = (FPRHIFvf & FPRHIFVF_TEXCOUNT_MASK);

    DWORD DXTextCount;

    switch (FPRHIPosType)
    {
        case  FPRHIFVF_TEX0:     DXTextCount = D3DFVF_TEX0; break;
        case  FPRHIFVF_TEX1:     DXTextCount = D3DFVF_TEX1; break;
        case  FPRHIFVF_TEX2:     DXTextCount = D3DFVF_TEX2; break;
        case  FPRHIFVF_TEX3:     DXTextCount = D3DFVF_TEX3; break;
        case  FPRHIFVF_TEX4:     DXTextCount = D3DFVF_TEX4; break;
        case  FPRHIFVF_TEX5:     DXTextCount = D3DFVF_TEX5; break;
        case  FPRHIFVF_TEX6:     DXTextCount = D3DFVF_TEX6; break;
        case  FPRHIFVF_TEX7:     DXTextCount = D3DFVF_TEX7; break;
        case  FPRHIFVF_TEX8:     DXTextCount = D3DFVF_TEX8; break;
        default:                 DXTextCount = 0x000; break;
    }

    //LASTBETA
    DWORD FPRHILastBeta = (FPRHIFvf ^ 0x0fff);

    DWORD DXLastBeta;

    switch (FPRHIPosType)
    {
    case  FPRHIFVF_LASTBETA_UBYTE4:      DXLastBeta = D3DFVF_LASTBETA_UBYTE4; break;
    case  FPRHIFVF_TEX1:                 DXLastBeta = D3DFVF_LASTBETA_D3DCOLOR; break;
    default:                             DXLastBeta = 0x000; break;
    }

    return (D3DPosType | DXNormal | DXPsize | DXDiffuse | DXSpecular | DXTextCount | DXLastBeta);
}

//FPRHIRENDERSTATETYPE To D3DRENDERSTATETYPE 타입 변환기
D3DRENDERSTATETYPE ChangeD3DRENDERSTATETYPE(FPRHIRENDERSTATETYPE FPRHIRenderStateType)
{
    switch (FPRHIRenderStateType)
    {
    case FPRHIRS_ZENABLE:                   return D3DRS_ZENABLE;
    case FPRHIRS_FILLMODE:                  return D3DRS_FILLMODE;
    case FPRHIRS_SHADEMODE:                 return D3DRS_SHADEMODE;
    case FPRHIRS_ZWRITEENABLE:              return D3DRS_ZWRITEENABLE;
    case FPRHIRS_ALPHATESTENABLE:           return D3DRS_ALPHATESTENABLE;
    case FPRHIRS_LASTPIXEL:                 return D3DRS_LASTPIXEL;
    case FPRHIRS_SRCBLEND:                  return D3DRS_SRCBLEND;
    case FPRHIRS_DESTBLEND:                 return D3DRS_DESTBLEND;
    case FPRHIRS_CULLMODE:                  return D3DRS_CULLMODE;
    case FPRHIRS_ZFUNC:                     return D3DRS_ZFUNC;
    case FPRHIRS_ALPHAREF:                  return D3DRS_ALPHAREF;
    case FPRHIRS_ALPHAFUNC:                 return D3DRS_ALPHAFUNC;
    case FPRHIRS_DITHERENABLE:              return D3DRS_DITHERENABLE;
    case FPRHIRS_ALPHABLENDENABLE:          return D3DRS_ALPHABLENDENABLE;
    case FPRHIRS_FOGENABLE:                 return D3DRS_FOGENABLE;
    case FPRHIRS_SPECULARENABLE:            return D3DRS_SPECULARENABLE;
    case FPRHIRS_FOGCOLOR:                  return D3DRS_FOGCOLOR;
    case FPRHIRS_FOGTABLEMODE:              return D3DRS_FOGTABLEMODE;
    case FPRHIRS_FOGSTART:                  return D3DRS_FOGSTART;
    case FPRHIRS_FOGEND:                    return D3DRS_FOGEND;
    case FPRHIRS_FOGDENSITY:                return D3DRS_FOGDENSITY;
    case FPRHIRS_RANGEFOGENABLE:            return D3DRS_RANGEFOGENABLE;
    case FPRHIRS_STENCILENABLE:             return D3DRS_STENCILENABLE;
    case FPRHIRS_STENCILFAIL:               return D3DRS_STENCILFAIL;
    case FPRHIRS_STENCILZFAIL:              return D3DRS_STENCILZFAIL;
    case FPRHIRS_STENCILPASS:               return D3DRS_STENCILPASS;
    case FPRHIRS_STENCILFUNC:               return D3DRS_STENCILFUNC;
    case FPRHIRS_STENCILREF:                return D3DRS_STENCILREF;
    case FPRHIRS_STENCILMASK:               return D3DRS_STENCILMASK;
    case FPRHIRS_STENCILWRITEMASK:          return D3DRS_STENCILWRITEMASK;
    case FPRHIRS_TEXTUREFACTOR:             return D3DRS_TEXTUREFACTOR;
    case FPRHIRS_WRAP0:                     return D3DRS_WRAP0;
    case FPRHIRS_WRAP1:                     return D3DRS_WRAP1;
    case FPRHIRS_WRAP2:                     return D3DRS_WRAP2;
    case FPRHIRS_WRAP3:                     return D3DRS_WRAP3;
    case FPRHIRS_WRAP4:                     return D3DRS_WRAP4;
    case FPRHIRS_WRAP5:                     return D3DRS_WRAP5;
    case FPRHIRS_WRAP6:                     return D3DRS_WRAP6;
    case FPRHIRS_WRAP7:                     return D3DRS_WRAP7;
    case FPRHIRS_CLIPPING:                  return D3DRS_CLIPPING;
    case FPRHIRS_LIGHTING:                  return D3DRS_LIGHTING;
    case FPRHIRS_AMBIENT:                   return D3DRS_AMBIENT;
    case FPRHIRS_FOGVERTEXMODE:             return D3DRS_FOGVERTEXMODE;
    case FPRHIRS_COLORVERTEX:               return D3DRS_COLORVERTEX;
    case FPRHIRS_LOCALVIEWER:               return D3DRS_LOCALVIEWER;
    case FPRHIRS_NORMALIZENORMALS:          return D3DRS_NORMALIZENORMALS;
    case FPRHIRS_DIFFUSEMATERIALSOURCE:     return D3DRS_DIFFUSEMATERIALSOURCE;
    case FPRHIRS_SPECULARMATERIALSOURCE:    return D3DRS_SPECULARMATERIALSOURCE;
    case FPRHIRS_AMBIENTMATERIALSOURCE:     return D3DRS_AMBIENTMATERIALSOURCE;
    case FPRHIRS_EMISSIVEMATERIALSOURCE:    return D3DRS_EMISSIVEMATERIALSOURCE;
    case FPRHIRS_VERTEXBLEND:               return D3DRS_VERTEXBLEND;
    case FPRHIRS_CLIPPLANEENABLE:           return D3DRS_CLIPPLANEENABLE;
    case FPRHIRS_POINTSIZE:                 return D3DRS_POINTSIZE;
    case FPRHIRS_POINTSIZE_MIN:             return D3DRS_POINTSIZE_MIN;
    case FPRHIRS_POINTSPRITEENABLE:         return D3DRS_POINTSPRITEENABLE;
    case FPRHIRS_POINTSCALEENABLE:          return D3DRS_POINTSCALEENABLE;
    case FPRHIRS_POINTSCALE_A:              return D3DRS_POINTSCALE_A;
    case FPRHIRS_POINTSCALE_B:              return D3DRS_POINTSCALE_B;
    case FPRHIRS_POINTSCALE_C:              return D3DRS_POINTSCALE_C;
    case FPRHIRS_MULTISAMPLEANTIALIAS:      return D3DRS_MULTISAMPLEANTIALIAS;
    case FPRHIRS_MULTISAMPLEMASK:           return D3DRS_MULTISAMPLEMASK;
    case FPRHIRS_PATCHEDGESTYLE:            return D3DRS_PATCHEDGESTYLE;
    case FPRHIRS_DEBUGMONITORTOKEN:         return D3DRS_DEBUGMONITORTOKEN;
    case FPRHIRS_POINTSIZE_MAX:             return D3DRS_POINTSIZE_MAX;
    case FPRHIRS_INDEXEDVERTEXBLENDENABLE:  return D3DRS_INDEXEDVERTEXBLENDENABLE;
    case FPRHIRS_COLORWRITEENABLE:          return D3DRS_COLORWRITEENABLE;
    case FPRHIRS_TWEENFACTOR:               return D3DRS_TWEENFACTOR;
    case FPRHIRS_BLENDOP:                   return D3DRS_BLENDOP;
    case FPRHIRS_POSITIONDEGREE:            return D3DRS_POSITIONDEGREE;
    case FPRHIRS_NORMALDEGREE:              return D3DRS_NORMALDEGREE;
    case FPRHIRS_SCISSORTESTENABLE:         return D3DRS_SCISSORTESTENABLE;
    case FPRHIRS_SLOPESCALEDEPTHBIAS:       return D3DRS_SLOPESCALEDEPTHBIAS;
    case FPRHIRS_ANTIALIASEDLINEENABLE:     return D3DRS_ANTIALIASEDLINEENABLE;
    case FPRHIRS_MINTESSELLATIONLEVEL:      return D3DRS_MINTESSELLATIONLEVEL;
    case FPRHIRS_MAXTESSELLATIONLEVEL:      return D3DRS_MAXTESSELLATIONLEVEL;
    case FPRHIRS_ADAPTIVETESS_X:            return D3DRS_ADAPTIVETESS_X;
    case FPRHIRS_ADAPTIVETESS_Y:            return D3DRS_ADAPTIVETESS_Y;
    case FPRHIRS_ADAPTIVETESS_Z:            return D3DRS_ADAPTIVETESS_Z;
    case FPRHIRS_ADAPTIVETESS_W:            return D3DRS_ADAPTIVETESS_W;
    case FPRHIRS_ENABLEADAPTIVETESSELLATION:return D3DRS_ENABLEADAPTIVETESSELLATION;
    case FPRHIRS_TWOSIDEDSTENCILMODE:       return D3DRS_TWOSIDEDSTENCILMODE;
    case FPRHIRS_CCW_STENCILFAIL:           return D3DRS_CCW_STENCILFAIL;
    case FPRHIRS_CCW_STENCILZFAIL:          return D3DRS_CCW_STENCILZFAIL;
    case FPRHIRS_CCW_STENCILPASS:           return D3DRS_CCW_STENCILPASS;
    case FPRHIRS_CCW_STENCILFUNC:           return D3DRS_CCW_STENCILFUNC;
    case FPRHIRS_COLORWRITEENABLE1:         return D3DRS_COLORWRITEENABLE1;
    case FPRHIRS_COLORWRITEENABLE2:         return D3DRS_COLORWRITEENABLE2;
    case FPRHIRS_COLORWRITEENABLE3:         return D3DRS_COLORWRITEENABLE3;
    case FPRHIRS_BLENDFACTOR:               return D3DRS_BLENDFACTOR;
    case FPRHIRS_SRGBWRITEENABLE:           return D3DRS_SRGBWRITEENABLE;
    case FPRHIRS_DEPTHBIAS:                 return D3DRS_DEPTHBIAS;
    case FPRHIRS_WRAP8:                     return D3DRS_WRAP8;
    case FPRHIRS_WRAP9:                     return D3DRS_WRAP9;
    case FPRHIRS_WRAP10:                    return D3DRS_WRAP10;
    case FPRHIRS_WRAP11:                    return D3DRS_WRAP11;
    case FPRHIRS_WRAP12:                    return D3DRS_WRAP12;
    case FPRHIRS_WRAP13:                    return D3DRS_WRAP13;
    case FPRHIRS_WRAP14:                    return D3DRS_WRAP14;
    case FPRHIRS_WRAP15:                    return D3DRS_WRAP15;
    case FPRHIRS_SEPARATEALPHABLENDENABLE:  return D3DRS_SEPARATEALPHABLENDENABLE;
    case FPRHIRS_SRCBLENDALPHA:             return D3DRS_SRCBLENDALPHA;
    case FPRHIRS_DESTBLENDALPHA:            return D3DRS_DESTBLENDALPHA;
    case FPRHIRS_BLENDOPALPHA:              return D3DRS_BLENDOPALPHA;
    case FPRHIRS_FORCE_DWORD:               return D3DRS_FORCE_DWORD;
    default:                                return D3DRS_FORCE_DWORD;
    }
}

//FPRHICULL To D3DCULL 타입 변환기
D3DCULL ChangeD3DCULL(DWORD FPRHICull)
{
    switch (FPRHICull)
    {
    case FPRHICULL_NONE:        return D3DCULL_NONE;
    case FPRHICULL_CW:          return D3DCULL_CW;
    case FPRHICULL_CCW:         return D3DCULL_CCW;
    case FPRHICULL_FORCE_DWORD: return D3DCULL_FORCE_DWORD;

    default:                    return D3DCULL_NONE;
    }
}

//FPRHIFILLMODE To D3DFILLMODE 타입 변환기
D3DFILLMODE ChangeD3DFILLMODE(DWORD FPRHIFillMode)
{
    switch (FPRHIFillMode)
    {
    case FPRHIFILL_POINT:           return D3DFILL_POINT;
    case FPRHIFILL_WIREFRAME:       return D3DFILL_WIREFRAME;
    case FPRHIFILL_SOLID:           return D3DFILL_SOLID;
    case FPRHIFILL_FORCE_DWORD:     return D3DFILL_FORCE_DWORD;

    default:                        return D3DFILL_POINT;
    }
}

//FPRHIUsages To D3DUsage 타입 변환기
long ChangeD3DUsage(long FPRHIUsage)
{
    switch (FPRHIUsage)
    {
    case FPRHIUSAGE_RENDERTARGET: return D3DUSAGE_RENDERTARGET;
    case FPRHIUSAGE_DEPTHSTENCIL:	return D3DUSAGE_DEPTHSTENCIL;
    case FPRHIUSAGE_DYNAMIC:	return D3DUSAGE_DYNAMIC;

    case FPRHIUSAGE_AUTOGENMIPMAP:	return D3DUSAGE_AUTOGENMIPMAP;
    case FPRHIUSAGE_DMAP:	return D3DUSAGE_DMAP;

    case FPRHIUSAGE_WRITEONLY:   return D3DUSAGE_WRITEONLY;
    case FPRHIUSAGE_SOFTWAREPROCESSING: return D3DUSAGE_SOFTWAREPROCESSING;
    case FPRHIUSAGE_DONOTCLIP: return D3DUSAGE_DONOTCLIP;
    case FPRHIUSAGE_POINTS: return D3DUSAGE_POINTS;
    case FPRHIUSAGE_RTPATCHES: return D3DUSAGE_RTPATCHES;
    case FPRHIUSAGE_NPATCHES: return D3DUSAGE_RTPATCHES;
    default: return 0;

    }
}

//FPRHIPOOL To D3DPOOL 타입 변환기
D3DPOOL ChangeD3DPOOL(FPRHIPOOL FPRHIPool)
{
    switch (FPRHIPool)
    {
    case FPRHIPOOL_DEFAULT:           return D3DPOOL_DEFAULT;
    case FPRHIPOOL_MANAGED:       return D3DPOOL_MANAGED;
    case FPRHIPOOL_SYSTEMMEM:           return D3DPOOL_SYSTEMMEM;
    case FPRHIPOOL_SCRATCH:     return D3DPOOL_SCRATCH;
    case FPRHIPOOL_FORCE_DWORD:     return D3DPOOL_FORCE_DWORD;

    default:                        return D3DPOOL_DEFAULT;
    }
}

//FPRHIPRIMITIVETYPE To D3DPRIMITIVETYPE 타입 변환기
D3DPRIMITIVETYPE ChangeD3DPRIMITIVETYPE(FPRHIPRIMITIVETYPE FPRHIPrmititiveType)
{
    switch (FPRHIPrmititiveType)
    {
    case FPRHIPT_POINTLIST:       return D3DPT_POINTLIST;
    case FPRHIPT_LINELIST:        return D3DPT_LINELIST;
    case FPRHIPT_LINESTRIP:       return D3DPT_LINESTRIP;
    case FPRHIPT_TRIANGLELIST:    return D3DPT_TRIANGLELIST;
    case FPRHIPT_TRIANGLESTRIP:   return D3DPT_TRIANGLESTRIP;
    case FPRHIPT_TRIANGLEFAN:     return D3DPT_TRIANGLEFAN;
    case FPRHIPT_FORCE_DWORD:     return D3DPT_FORCE_DWORD;

    default:                        return D3DPT_FORCE_DWORD;
    }
}

//////////////////////
// DXVertexBufferImpl
class DXVertexBufferImpl
{
    private:
        Comptr<IDirect3DVertexBuffer9> VertexBuffer;

    public:
        DXVertexBufferImpl();
        ~DXVertexBufferImpl();

        //COM 객체 반환
        IDirect3DVertexBuffer9** GetVertexBuffer() { return VertexBuffer.GetAddressOf(); };

        HRESULT Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
        HRESULT Unlock();

};

DXVertexBufferImpl::DXVertexBufferImpl()
{

}

DXVertexBufferImpl::~DXVertexBufferImpl()
{

}

HRESULT DXVertexBufferImpl::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
    VertexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
    return S_OK;
}

HRESULT DXVertexBufferImpl::Unlock()
{
    VertexBuffer->Unlock();
    return S_OK;
}

//////////////////////
// DXDeviceImpl
class DXDeviceImpl
{
    private:
        Comptr<IDirect3DDevice9> Device;

    public:
        DXDeviceImpl();
        ~DXDeviceImpl();
        //COM객체 반환
        IDirect3DDevice9** GetDevice() { return Device.GetAddressOf(); };

        HRESULT BeginScene();
        HRESULT Clear(DWORD Count, const FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil);
        HRESULT DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
        HRESULT EndScene();
        HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

        HRESULT GetDC(HDC* phdc);
        HRESULT ReleaseDC(HDC hdc);

        HRESULT SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value);
        HRESULT GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue);


        HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffedr, HANDLE* pSharedHandle);
        HRESULT SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride);
        HRESULT SetFVF(DWORD FVF);
};

DXDeviceImpl::DXDeviceImpl() = default;

DXDeviceImpl::~DXDeviceImpl() = default;

HRESULT DXDeviceImpl::BeginScene()
{
    Device->BeginScene();
    return S_OK;
}

HRESULT DXDeviceImpl::Clear(DWORD Count, const FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil)
{
    D3DRECT D3Drect;
    ChangeD3DRECT(pRects, D3Drect);
    D3DCOLOR D3DColor;
    ChangeD3DCOLOR(Color, D3DColor);
    Device->Clear(
        Count,
        pRects != nullptr ? &D3Drect : nullptr,
        Flags,
        D3DColor,
        Z,
        Stencil
    );
    return S_OK;
}

HRESULT DXDeviceImpl::DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    Device->DrawPrimitive(ChangeD3DPRIMITIVETYPE(PrimitiveType), StartVertex, PrimitiveCount);
    return S_OK;
}

HRESULT DXDeviceImpl::EndScene()
{
    HRESULT res = Device->EndScene();
    return S_OK;
}

HRESULT DXDeviceImpl::Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
    Device->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    return S_OK;
}

HRESULT DXDeviceImpl::GetDC(HDC* phdc)
{
    IDirect3DSurface9* backBuffer = nullptr;

    HRESULT hr = Device->GetBackBuffer(
        0,                          // SwapChain index
        0,                          // BackBuffer index
        D3DBACKBUFFER_TYPE_MONO,
        &backBuffer
    );

    if (FAILED(hr))
        return hr;

    hr = backBuffer->GetDC(phdc);
    backBuffer->Release();

}

HRESULT DXDeviceImpl::ReleaseDC(HDC hdc)
{
    IDirect3DSurface9* backBuffer = nullptr;

    HRESULT hr = Device->GetBackBuffer(
        0,                          // SwapChain index
        0,                          // BackBuffer index
        D3DBACKBUFFER_TYPE_MONO,
        &backBuffer
    );

    if (FAILED(hr))
        return hr;
    backBuffer->ReleaseDC(hdc);
    backBuffer->Release();
}

HRESULT DXDeviceImpl::SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value)
{
    D3DRENDERSTATETYPE D3DRenderStateType = ChangeD3DRENDERSTATETYPE(State);
    if (D3DRenderStateType == D3DRS_CULLMODE)
    {
        Device->SetRenderState(D3DRenderStateType, ChangeD3DCULL(Value));
    }

    if (D3DRenderStateType == D3DRS_FILLMODE)
    {
        Device->SetRenderState(D3DRenderStateType, ChangeD3DFILLMODE(Value));
    }
    return S_OK;
}

HRESULT DXDeviceImpl::GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue)
{
    Device->GetRenderState(ChangeD3DRENDERSTATETYPE(State), pValue);
    return S_OK;
}


HRESULT DXDeviceImpl::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffedr, HANDLE* pSharedHandle)
{
    HRESULT hr = Device->CreateVertexBuffer(Length, ChangeD3DUsage(Usage), ChangeD3DFVF_FORMAT(FVF), ChangeD3DPOOL(Pool), ppVertexBuffedr, pSharedHandle);

    return S_OK;
}

HRESULT DXDeviceImpl::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
    Device->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
    return S_OK;
}

HRESULT DXDeviceImpl::SetFVF(DWORD FVF)
{
    Device->SetFVF(ChangeD3DFVF_FORMAT(FVF));
    return S_OK;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////
// DXImpl
// DX pimpl 패턴 적용
class DXImpl
{
    private:
        Comptr<IDirect3D9> D3D;

    public:
        DXImpl(UINT DeviceVersion);
        ~DXImpl() = default;
        
        //Com객체 반환
        IDirect3D9** GetD3D() { return D3D.GetAddressOf(); };

        //Device 생성
        HRESULT CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);
};

DXImpl::DXImpl(UINT DeviceVersion)
{
    //DX 객체 만들기
    D3D.Attach(Direct3DCreate9(DeviceVersion));

    //생성 되었는지 체크 필요
}

//DXDevice 생성
HRESULT DXImpl::CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
    D3DDEVTYPE D3DDevice = ChangeD3DDEVTYPE(DeviceType);
    D3DPRESENT_PARAMETERS D3DPP;
    ZeroMemory(&D3DPP, sizeof(D3DPP));
    ChangeD3DPRESENT_PARAMETERS(D3DPP, pPresentationParameters);

    HRESULT res = D3D->CreateDevice(Adapter, D3DDevice, hFocusWindow, BehaviorFlags, &D3DPP, ppReturnedDeviceInterface);
    if (FAILED(res))
    {
        return E_FAIL;
    }

    return S_OK;

}





//////////////////////
// DXRHI
DXRHI::DXRHI(UINT DeviceVersion) : DeviceVersion(DeviceVersion), DXimpl(std::make_unique<DXImpl>(DeviceVersion)) {}

DXRHI::~DXRHI() = default;

//DXDevice 생성
HRESULT DXRHI::CreateDevice(UINT Adapter, FPRHIDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, FPRHIPRESENT_PARAMETERS* pPresentationParameters, FPRHIDevice** ppReturnedDeviceInterface)
{
    DXRHIDevice* Device = new DXRHIDevice();
    HRESULT res = DXimpl->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, Device->DXDeviceimpl->GetDevice());
    if (FAILED(res))
    {
        return E_FAIL;
    }

    *ppReturnedDeviceInterface = Device;

    return S_OK;
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////
// DXRHIDevice

DXRHIDevice::DXRHIDevice():DXDeviceimpl(std::make_unique<DXDeviceImpl>())
{
}

DXRHIDevice::~DXRHIDevice() = default;

HRESULT DXRHIDevice::BeginScene()
{
    DXDeviceimpl->BeginScene();
	return S_OK;
}

HRESULT DXRHIDevice::Clear(DWORD Count, const FPRHIRECT* pRects, DWORD Flags, FPRHICOLOR Color, float Z, DWORD Stencil)
{
    DXDeviceimpl->Clear(Count, pRects, Flags, Color, Z, Stencil);
	return S_OK;
}

HRESULT DXRHIDevice::DrawPrimitive(FPRHIPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    DXDeviceimpl->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
    return S_OK;
}

HRESULT DXRHIDevice::EndScene()
{
    DXDeviceimpl->EndScene();
	return S_OK;
}


HRESULT DXRHIDevice::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
    DXDeviceimpl->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return S_OK;
}

HRESULT DXRHIDevice::GetDC(HDC* phdc)
{
    DXDeviceimpl->GetDC(phdc);
    return S_OK;
}

HRESULT DXRHIDevice::ReleaseDC(HDC hdc)
{
    DXDeviceimpl->ReleaseDC(hdc);
    return S_OK;
}

HRESULT DXRHIDevice::SetRenderState(FPRHIRENDERSTATETYPE State, DWORD Value)
{
    DXDeviceimpl->SetRenderState(State, Value);
    return S_OK;
}

HRESULT DXRHIDevice::GetRenderState(FPRHIRENDERSTATETYPE State, DWORD* pValue)
{
    DXDeviceimpl->GetRenderState(State, pValue);
    return S_OK;
}

HRESULT DXRHIDevice::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, FPRHIPOOL Pool, FPRHIVertexBuffer** ppVertexBuffedr, HANDLE* pSharedHandle)
{
    DXRHIVertexBuffer* DXVB = new DXRHIVertexBuffer();
    DXDeviceimpl->CreateVertexBuffer(Length, Usage, FVF, Pool, DXVB->DXVertexBufferimpl->GetVertexBuffer() , pSharedHandle);
    *ppVertexBuffedr = DXVB;
    return S_OK;
}

HRESULT DXRHIDevice::SetStreamSource(UINT StreamNumber, FPRHIVertexBuffer* pStreamData, UINT OffsetInBytes, UINT Stride)
{
    DXRHIVertexBuffer* DXVB = dynamic_cast<DXRHIVertexBuffer*>(pStreamData);
    DXDeviceimpl->SetStreamSource(StreamNumber, *(DXVB->DXVertexBufferimpl->GetVertexBuffer()), OffsetInBytes, Stride);
    return S_OK;
}

HRESULT DXRHIDevice::SetFVF(DWORD FVF)
{
    DXDeviceimpl->SetFVF(FVF);
    return S_OK;
}



//////////////////////
// DXRHIVertexBuffer

DXRHIVertexBuffer::DXRHIVertexBuffer() :DXVertexBufferimpl(std::make_unique<DXVertexBufferImpl>())
{
}

DXRHIVertexBuffer::~DXRHIVertexBuffer()
{
}

HRESULT DXRHIVertexBuffer::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
    DXVertexBufferimpl->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
    return S_OK;
}

HRESULT DXRHIVertexBuffer::Unlock()
{
    DXVertexBufferimpl->Unlock();
    return S_OK;
}
