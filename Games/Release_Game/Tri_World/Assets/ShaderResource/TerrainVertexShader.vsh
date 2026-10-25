//
// 기본 VertexShader 소스

//Lambert Light
#include "LambertLight.hlsli"

//상수 버퍼
cbuffer ObejctConstBuffer : register(b0)
{
    matrix mWorld;
    matrix mView;
    matrix mProj;
    matrix mWV;
    matrix mWVP;
};

cbuffer ViewPortConstBuffer : register(b1)
{

};

struct LIGHT
{
    float4 Direction;
    float Range;
    float4 Diffuse;
    float4 Ambient;
    float4 LightColor;
};

cbuffer DirectionLightConstBuffer : register(b2)
{
    LIGHT Light;
};

struct MATERIAL
{
    float4 BaseColor;
    float4 Diffuse;
    float4 Ambient;
};

cbuffer MaterialConstBuffer : register(b3)
{
    MATERIAL Material;
};

//VS 출력 구조체
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
};
 

////////////////////////////////////////////////////////////////////////////// 
//
//! Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

VSOutput VS_Main(
                float4 pos : POSITION, //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
                float4 nrm : NORMAL, //[입력] 노멀 normal
                float4 col : COLOR0, //[입력] 정점색. Vertex Color : "Diffuse"
                float2 uv : TEXCOORD0 //[입력] UV
                )
{
    //입력된 정보 그대로 출력..
    VSOutput o = (VSOutput) 0;

    //변환
    pos = mul(mWVP, pos);

    float4 Diff = 1;
    
    float4 N = nrm;
    float4 L = Light.Direction;
    
    
    Diff = LambertLight(N, L, Light.Diffuse, Light.Ambient, Material.Diffuse, Material.Ambient);
    
    o.pos = pos;
    o.col = Light.LightColor;
    //o.col = Material.BaseColor * Light.LightColor * Diff;
    //o.col = Material.BaseColor * Diff;
    
    uv.x = uv.x * 20;
    uv.y = uv.y * 20;
    
    o.uv = uv;
    
    return o;
}
