//////////////////////////////////
//
// Per Pixel Light 운용 셰이더 코드
//
//

#ifndef PIXEL_LIGHT_HLSLI
#define PIXEL_LIGHT_HLSLI

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

//텍스처 객체 변수 : 엔진에서 공급됨, 레지스터 직접 지정. (기본값은 t0)
Texture2D EnvTex : register(t127);

//텍스처 셈플러 (직접지정)  기본값은 s0
SamplerState EnvSampler : register(s15);

cbuffer SkyLightConstantBuffer : register(b1)
{
    float4 LightColor;
    float LightIntensity;
};


    //환경광 텍스처 좌표 계산
static const float4x4 MatrixEnv =
{
    0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, -0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
};


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
    uint LightCnt; //바인딩된 Light 개수
    float3 Padding;
    
    LIGHT Light[Light_MAX]; //Light를 최대 개수만큼 배열 생성 144B * 64 = 9216B
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
void CalculateLight(float4 posv, float4 nrm, matrix mView, out float4 OutDiffuse, out float4 OutSpecular, out float4 OutEmissive);

//조명 계산 함수
void DirLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular);
void PointLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular);
void SpotLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular);
void RectLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular);
void EnvLight(float4 nrm, inout float4 OutAmbient);

void CalculateLight(float4 posv, float4 nrm, matrix mView, out float4 OutDiffuse, out float4 OutSpecular, out float4 OutEmissive, out float4 OutAmbient)
{
    OutDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    OutSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    OutEmissive = (Material.Emissive * Material.BaseColor);
    OutAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (uint i = 0; i < min(LightCnt, (uint) Light_MAX); ++i)
    {
        switch (Light[i].LightType)
        {
            case LightType_Direction:
                DirLight(posv, nrm, mView, Light[i], OutDiffuse, OutSpecular);
                break;
            
            case LightType_Point:
                PointLight(posv, nrm, mView, Light[i], OutDiffuse, OutSpecular);
                break;
            
            case LightType_Spot:
                //Col += SpotLight(posv, nrm, mView, Light[i], OutDiffuse, OutSpecular);
                break;
            
            case LightType_Rect:
                //Col += RectLight(posv, nrm, mView, Light[i], OutDiffuse, OutSpecular);
                break;
        }
    }
    
    EnvLight(nrm, OutAmbient);

}


void DirLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular)
{
    float4 Diff = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Amb = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 Normal = float4(nrm.xyz, 0.0f);
    float4 N = Normal;
    N.w = 0; //4성분 되면서 w가 0이 아니게 될 수 있음.
    N = normalize(N);
    
    float4 L = float4(LightData.Direction.xyz, 0.0f);
    L = mul(mView, L);
    L.w = 0.0f;
    L = normalize(L);
    
    posv.w = 0;
    float4 V = normalize(-(posv));
    
    
    Diff = LambertLight(N, L, LightData.Diffuse, Material.Diffuse) * Material.BaseColor;
    Spec = SpecularBlinPhongLight(N, L, V, LightData.Specular, Material.Specular, Material.Roughness);
    Amb = LightData.Ambient * Material.Ambient * Material.BaseColor;
    
    OutDiffuse = OutDiffuse + ((Diff + Amb) * LightData.LightColor);
    OutSpecular = OutSpecular + Spec * LightData.LightColor;

}

void PointLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular)
{
    float4 Diff = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Amb = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 ViewLightPosition = mul(mView, LightData.LightPosition);
    float4 L = float4((ViewLightPosition.xyz - posv.xyz), 0.0f);
    
    float4 Normal = float4(nrm.xyz, 0.0f);
    float4 N = Normal;
    N.w = 0;
    N = normalize(N);
    
    posv.w = 0;
    float4 V = normalize(-(posv));
    
    float Distance = max(length(L), 0.01);
    
    //광원 도달 거리 벗어남
    if (Distance > LightData.Range)
        return;
    
    //정규화
    L /= Distance;
    
    //거리 감쇠 공식
    float Under_att = (LightData.Attenuation.x + LightData.Attenuation.y * Distance + LightData.Attenuation.z * Distance * Distance);
    
    float att = 1 / max(Under_att, 0.0001f);
    
    //Ambient SmoothStep
    float Fade = 1.0f - smoothstep(LightData.Range * 0.8f, LightData.Range, Distance);

    
    Diff = LambertLight(N, L, LightData.Diffuse, Material.Diffuse) * att * Fade * Material.BaseColor;
    Spec = SpecularBlinPhongLight(N, L, V, LightData.Specular, Material.Specular, Material.Roughness) * att * Fade;
    Amb = LightData.Ambient * Material.Ambient * att * Fade * Material.BaseColor;
    
    OutDiffuse = OutDiffuse + ((Diff + Amb) * LightData.LightColor);
    OutSpecular = OutSpecular + Spec * LightData.LightColor;
}

void SpotLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular)
{
    //미구현이므로 Light 연산을 하지 않음
    float4 Col = { 0.0f, 0.0f, 0.0f, 0.0f };
    OutDiffuse = OutDiffuse + Col;
    OutSpecular = OutSpecular + Col;
}

void RectLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular)
{
    //미구현이므로 Light 연산을 하지 않음
    float4 Col = { 0.0f, 0.0f, 0.0f, 0.0f };
    OutDiffuse = OutDiffuse + Col;
    OutSpecular = OutSpecular + Col;
}

void EnvLight(float4 nrm, inout float4 OutAmbient)
{
    float4 Normal = float4(nrm.xyz, 0.0f);
    float4 N = Normal;
    N.w = 0;
    N = normalize(N);
   
    float2 EnvUV = mul(MatrixEnv, float4(N.xyz, 1.0f)).xy;

    
    
    OutAmbient = EnvTex.Sample(EnvSampler, EnvUV) * LightColor * LightIntensity * Material.BaseColor;
}
#endif