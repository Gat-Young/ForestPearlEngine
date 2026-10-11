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

cbuffer DirectionLightConstBuffer : register(b2)
{
    float3 LightDirection;
    float LightRange;
    float4 LightDiffuse;
    float4 LightAmbient;
};

cbuffer MaterialConstBuffer : register(b3)
{
    float4 MaterialBaseColor;
    float4 MaterialDiffuse;
    float4 MaterialAmbient;
};

//VS 출력 구조체
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
};
 

////////////////////////////////////////////////////////////////////////////// 
//
//! Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

VSOutput VS_Main(
                float4 pos : POSITION,       //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
                float4 nrm : NORMAL,         //[입력] 노멀 normal
                float4 col : COLOR0,         //[입력] 정점색. Vertex Color : "Diffuse"
                float2 uv : TEXCOORD0        //[입력] UV
                )
{
    //입력된 정보 그대로 출력..
    VSOutput o = (VSOutput) 0;

    //변환
    pos = mul(mWVP, pos);

    float4 Diff = 1;
    
    float4 N = nrm;
    N.w = 0;
    N = mul(mWV, N);
    N = normalize(N);
    
    float4 L = float4(LightDirection, 0);
    L = mul(mView, L);
    L = normalize(L);
    
    
    Diff = LambertLight(N, L, LightDiffuse, LightAmbient, MaterialDiffuse, MaterialAmbient);
    
    o.pos = pos;
    o.col = MaterialBaseColor * Diff;
    
    return o;
}
