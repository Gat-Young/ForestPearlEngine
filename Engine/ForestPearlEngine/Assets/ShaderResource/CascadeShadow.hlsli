#ifndef CASCADE_SHADOW_HLSLI
#define CASCADE_SHADOW_HLSLI

//Cascade ShadowMap
//텍스처 배열 형태 : 각각의 레이어가 Cascade 1개에 대응됨.
Texture2DArray<float> CascadeShadowMap : register(t126);

//텍스처 샘플러 (비교샘플러)  기본값은 s0
SamplerComparisonState ShadowSampler : register(s14);       //PCF Percentage Closer Filtering

//한 Light가 가질 수 있는 최대 Cascade 개수 (FPDirectionalLightComponent::SetCascadeCount()의 Clamp 값과 일치해야 함)
#define MAX_CASCADE 8

//Cascade Shadow에 필요한 상수버퍼 (Pixel Shader에서 Shadow 샘플링 시 사용)
cbuffer CascadeShadowConstBuffer : register(b13)
{
    float4x4 mLightViewProj[MAX_CASCADE];           //Shadow Map 투영 좌표 계산

    float CascadeNear[MAX_CASCADE];                 //Cascade 시작 깊이
    float CascadeFar[MAX_CASCADE];                  //Cascade 종료 깊이 및 선택
    float CascadeDepthRange[MAX_CASCADE];            //Light Projection의 Far-Near, Bias 정규화

    //HLSL은 float 배열 다음에 오는 스칼라를 배열 마지막 레지스터의 남는 12B에 끼워 넣어(Offset 884),
    //항상 16B 단위로 배치되는 C++ 구조체(CascadeCount가 Offset 896)와 어긋나 버림. 12B Padding으로 정렬을 맞춤
    float3 PaddingCascadeDepthRangeAlign;

    uint CascadeCount;                               //Cascade 개수
    float DynamicShadowDistance;                     //최대 그림자 적용 거리
    float CascadeTransitionFraction;                 //Cascade Blending

    float ShadowDistanceFadeoutFraction;             //거리 기반 Shadow Fade Out
    float ShadowBias;                                //깊이 비교 보정 (고정값)
    float ShadowSlopeBias;                           //Slope-Scaled Bias 계수
    uint CastShadow;                                  //그림자 적용 여부
    float PaddingCascadeShadow;                                   //16B 배수 유지용 Padding

    float3 LightDirectionView;                       //Slope-Scaled Bias 계산용 View Space 빛 방향
};

uint SelectCascade(float ViewDepth)
{

    for (uint i = 0; i < CascadeCount; ++i)
    {
        if (ViewDepth <= CascadeFar[i])
            return i;
    }

    return CascadeCount;
}

//Bias : 호출부에서 미리 계산한 (고정 Bias + Slope-Scaled Bias) 값
float SampleCascadeShadow(float4 WorldPos, uint Index, float Bias)
{
    float4 LightClip = mul(mLightViewProj[Index], WorldPos);

    float3 NDC = LightClip.xyz / LightClip.w;

    float2 UV;

    UV.x = NDC.x * 0.5f + 0.5f;
    UV.y = 0.5f - NDC.y * 0.5f;

    if (any(UV < 0.0f) || any(UV > 1.0f) || NDC.z < 0.0f || NDC.z > 1.0f)
    {
        return 1.0f;
    }

    //Cascade별 깊이 범위에 따른 Bias 정규화
    float NormalizedBias = Bias / max(CascadeDepthRange[Index], 0.000001f);
    float CompareDepth = NDC.z - NormalizedBias;

    //ShadowResolution을 몰라도 되도록 텍스처 크기를 직접 조회해 텍셀 크기 계산
    uint MapWidth, MapHeight, MapElements;
    CascadeShadowMap.GetDimensions(MapWidth, MapHeight, MapElements);
    float2 TexelSize = 1.0f / float2(MapWidth, MapHeight);

    //3x3 PCF : ShadowSampler 자체가 이미 2x2 하드웨어 Bilinear 비교 샘플링이라,
    //9탭을 더 섞으면 그림자 경계의 계단현상(Texel 격자가 그대로 비치는 현상)이 크게 줄어듦
    float Shadow = 0.0f;
    [unroll]
    for (int y = -1; y <= 1; ++y)
    {
        [unroll]
        for (int x = -1; x <= 1; ++x)
        {
            float2 Offset = float2(x, y) * TexelSize;
            Shadow += CascadeShadowMap.SampleCmpLevelZero(ShadowSampler, float3(UV + Offset, Index), CompareDepth);
        }
    }

    return Shadow / 9.0f;
}

//ViewNormal : 표면의 View Space Normal (Slope-Scaled Bias 계산용, 기존 Lighting에서 쓰는 N과 동일한 값을 그대로 넘기면 됨)
float CalculateShadow(float4 WorldPos, float ViewDepth, float3 ViewNormal)
{
    if (CastShadow == 0 || CascadeCount == 0) return 1.0f;

    if (CascadeCount > MAX_CASCADE) return 1.0f;

    uint Index = SelectCascade(ViewDepth);

    if (Index >= CascadeCount) return 1.0f;

    //빛과 표면이 이루는 각도가 커질수록(= 거의 평행해질수록) Bias를 더 키워서,
    //경사면에서 Shadow Map Texel 격자를 따라 생기는 계단형 자기 그림자(Self-Shadowing Terracing)를 줄임
    float NdotL = saturate(dot(normalize(ViewNormal), normalize(LightDirectionView)));
    float SlopeScale = sqrt(saturate(1.0f - NdotL * NdotL)) / max(NdotL, 0.05f);
    SlopeScale = min(SlopeScale, 3.0f);  //과도한 그림자 분리(Peter-Panning) 방지용 Clamp

    float TotalBias = ShadowBias + ShadowSlopeBias * SlopeScale;

    //현재 Cascade의 그림자 계산
    float ShadowFactor = SampleCascadeShadow(WorldPos, Index, TotalBias);

    //Cascade Blending
    //마지막 Cascade가 아닌 경우에만 처리
    if (Index + 1 < CascadeCount)
    {
        float BlendEnd = CascadeFar[Index];

        float BlendRange = (CascadeFar[Index] - CascadeNear[Index]) * saturate(CascadeTransitionFraction);

        float BlendStart = BlendEnd - BlendRange;

        //Blending 영역인 경우
        if (BlendRange > 0.0f && ViewDepth > BlendStart)
        {
            //다음 Cascade의 그림자 계산
            float NextShadow = SampleCascadeShadow(WorldPos, Index + 1, TotalBias);

            //Cascade Blending 비율 계산
            float BlendFactor = saturate( (ViewDepth - BlendStart) / BlendRange);

            ShadowFactor = lerp( ShadowFactor, NextShadow, BlendFactor);
        }
    }

    if (ViewDepth >= DynamicShadowDistance) return 1.0f;
    //거리 기반 Shadow Fade Out
    float FadeRange = DynamicShadowDistance * saturate(ShadowDistanceFadeoutFraction);

    float FadeStart = DynamicShadowDistance - FadeRange;

    if (FadeRange > 0.0f && ViewDepth > FadeStart)
    {
        float FadeFactor = saturate( (ViewDepth - FadeStart) / FadeRange);

        ShadowFactor = lerp(ShadowFactor, 1.0f, FadeFactor);
    }

    return ShadowFactor;
}
#endif
