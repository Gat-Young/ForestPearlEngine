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

    uint CascadeCount;                               //Cascade 개수
    uint ShadowResolution;                           //PCF의 Texel 크기 계산
    float DynamicShadowDistance;                     //최대 그림자 적용 거리
    float CascadeTransitionFraction;                 //Cascade Blending

    float ShadowDistanceFadeoutFraction;             //거리 기반 Shadow Fade Out
    float ShadowBias;                                //깊이 비교 보정
    uint CastShadow;                                  //그림자 적용 여부
    float Padding;
};

uint SelectCascade(float ViewDepth)
{
    uint CascadeIndex = 0;

    for (uint i = 0; i < CascadeCount; ++i)
    {
        if (ViewDepth <= CascadeFar[i])
            return i;
    }

    return CascadeCount;
}

#endif
