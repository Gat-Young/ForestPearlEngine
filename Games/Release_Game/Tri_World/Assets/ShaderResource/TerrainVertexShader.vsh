//
// 기본 VertexShader 소스

#include "Light.hlsli"

//상수 버퍼
cbuffer ObejctConstBuffer : register(b0)
{
    matrix mWorld;
    matrix mInverseWorld;
    matrix mTranseposeWorld;
    matrix mInverseTranseposeWorld;
    
    matrix mView;
    matrix mInverseView;
    matrix mTranseposeView;
    matrix mInverseTranseposeView;
    
    matrix mProj;
    matrix mInverseProj;
    matrix mTranseposeProj;
    matrix mInverseTranseposeProj;
    
    matrix mWV;
    matrix mInverseWV;
    matrix mTranseposeWV;
    matrix mInverseTranseposeWV;
    
    matrix mWVP;
    matrix mInverseWVP;
    matrix mTranseposeWVP;
    matrix mInverseTranseposeWVP;
    
};
cbuffer ViewPortConstBuffer : register(b1)
{

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

        
    //View 변환
    float4 posv = mul(mWV, pos);
    
    //변환
     pos = mul(mWVP, pos);
    
    o.pos = pos;
    o.col = CalculateLight(posv, nrm, mWV, mView, mInverseTranseposeWV);
    uv.x = uv.x * 20;
    uv.y = uv.y * 20;
    
    o.uv = uv;
    
    return o;
}