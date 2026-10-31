//////////////////////////////////
//
// 통합 Light 운용 셰이더 코드
//
//

#ifndef LIGHT_HLSLI
#define LIGHT_HLSLI

//Light 계산 함수
#include "LambertLight.hlsli"
#include "SpecularBlinPhong.hlsli"

//광원 종류
#define 	LightType_Direction 0
#define 	LightType_Point     1
#define 	LightType_Spot      2
#define 	LightType_Rect      3

//광원 최대 개수
#define     Light_MAX           64

struct LIGHT
{
    uint LightType;
    float3 Padding;
    float4 LightPosition;
    float4 Direction;
    float Range;
    float3 Padding_2;
    float4 Diffuse;
    float4 Ambient;
    float4 Specular;
    float4 Attenuation;
    float4 LightColor;
};

// 상수 버퍼크기 9232B
cbuffer LightConstBuffer : register(b2)
{
    uint LightCnt;                                  //바인딩된 Light 개수
    float3 Padding;
    
    LIGHT Light[Light_MAX];                         //Light를 최대 개수만큼 배열 생성 144B * 64 = 9216B
};

struct MATERIAL
{
    float4 BaseColor;
    float4 Diffuse;
    float4 Ambient;
    float4 Specular;
    float4 Roughness;
    float4 Emissive;
};

cbuffer MaterialConstBuffer : register(b3)
{
    MATERIAL Material;
};

//조명 처리 함수.
float4 CalculateLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV);

//조명 계산 함수
float4 DirLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV, LIGHT LightData);
float4 PointLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV, LIGHT LightData);
float4 SpotLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV, LIGHT LightData);
float4 RectLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV, LIGHT LightData);

float4 CalculateLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV)
{
    float4 Col = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    for (uint i = 0; i < min(LightCnt, (uint) Light_MAX); ++i)
    {
        switch (Light[i].LightType)
        {
	        case LightType_Direction :
                //Col += DirLight(posv, nrm, mWV, mView, mInverseTranseposeWV,Light[i]);
                Col = DirLight(posv, nrm, mWV, mView, mInverseTranseposeWV, Light[i]);
                break;
            
	        case LightType_Point     :
                Col += PointLight(posv, nrm, mWV, mView, mInverseTranseposeWV,Light[i]);
                break;
            
	        case LightType_Spot      :
                //Col += SpotLight(posv, nrm, mWV, mView, mInverseTranseposeWV,Light[i]);
                break;
            
	        case LightType_Rect      :
                //Col += RectLight(posv, nrm, mWV, mView, mInverseTranseposeWV, Light[i]);
                break;
        }
    }
    
    //return Col;
    
    float4 Result = (Col + Material.Emissive * Material.BaseColor);
    
    return float4(Result.rgb, Material.BaseColor.a);

}


float4 DirLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV, LIGHT LightData)
{
    float4 Diff = 1;
    float4 Spec = 0;
    float4 Amb = 0;
    
    float4 Normal = float4(nrm.xyz, 0.0f);
    float4 N = mul(mInverseTranseposeWV, Normal);
    N.w = 0; //4성분 되면서 w가 0이 아니게 될 수 있음.
    N = normalize(N);
    
    float4 L = float4(LightData.Direction.xyz, 0.0f);
    L = mul(mView, L);
    L.w = 0.0f;
    L = normalize(L);
    
    posv.w = 0;
    float4 V = normalize(-(posv));
    
    
    Diff = LambertLight(N, L, LightData.Diffuse, Material.Diffuse) * Material.BaseColor ;
    Spec = SpecularBlinPhongLight(N, L, V, LightData.Specular, Material.Specular, Material.Roughness);
    Amb = LightData.Ambient * Material.Ambient * Material.BaseColor;
    
    return saturate((Diff + Spec + Amb)) * LightData.LightColor;

    //return float4(L.xyz * 0.5f + 0.5f, 1.0f) + Diff * 0.000001f;
    
    //L = float4(normalize(LightData.Direction.xyz), 0.0f);

    //return float4(L.xyz * 0.5f + 0.5f, 1.0f);
    
    //L = float4(0.0f, 0.0f, -1.0f, 0.0f);

    //L = mul(mView, L);
    //L.w = 0.0f;
    //L = normalize(L);

    //return float4(L.xyz * 0.5f + 0.5f, 1.0f);
}

float4 PointLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV, LIGHT LightData)
{
    float4 Diff = 1;
    float4 Spec = 1;
    float4 Amb = 1;
    
    float4 ViewLightPosition = mul(mView, LightData.LightPosition);
    float4 L = float4((ViewLightPosition.xyz - posv.xyz), 0.0f);
    
    float4 Normal = float4(nrm.xyz, 0.0f);
    float4 N = mul(mInverseTranseposeWV, Normal);
    N.w = 0;
    N = normalize(N);
    
    posv.w = 0;
    float4 V = normalize(-(posv));
    
    float Distance = max(length(L), 0.01);
    
    //광원 도달 거리 벗어남
    if (Distance > LightData.Range)
        return 0;
    
    //정규화
    L /= Distance;
    
    //거리 감쇠 공식
    float Under_att = (LightData.Attenuation.x + LightData.Attenuation.y * Distance + LightData.Attenuation.z * Distance * Distance);
    
    float att = 1 / max(Under_att, 0.0001f);
    
    //Ambient SmoothStep
    float Fade = 1.0f - smoothstep( LightData.Range * 0.8f, LightData.Range, Distance);

    
    Diff = LambertLight(N, L, LightData.Diffuse, Material.Diffuse) * att * Fade * Material.BaseColor;
    Spec = SpecularBlinPhongLight(N, L, V, LightData.Specular, Material.Specular, Material.Roughness) * att * Fade;
    Amb = LightData.Ambient * Material.Ambient * Fade * Material.BaseColor;
    
    return saturate((Diff + Spec + Amb)) * LightData.LightColor;
}
float4 SpotLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV, LIGHT LightData)
{
    //미구현이므로 Light 연산을 하지 않음
    float4 Col = { 0.0f, 0.0f, 0.0f, 0.0f };
    return Col;
}
float4 RectLight(float4 posv, float4 nrm, matrix mWV, matrix mView, matrix mInverseTranseposeWV, LIGHT LightData)
{
    //미구현이므로 Light 연산을 하지 않음
    float4 Col = { 0.0f, 0.0f, 0.0f, 0.0f };
    return Col;
}

#endif