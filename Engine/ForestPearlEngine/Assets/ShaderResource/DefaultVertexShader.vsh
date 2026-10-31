//
// 기본 VertexShader 소스

// 통합 Light 운용 Shader
// b2 : Light,  b3 : Material
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
                float4 pos : POSITION,       //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
                float4 nrm : NORMAL,         //[입력] 노멀 normal
                float2 uv : TEXCOORD0        //[입력] UV
                )
{
    //입력된 정보 그대로 출력..
    VSOutput o = (VSOutput) 0;

        
    //View 변환
    float4 posv = mul(mWV, pos);
    
    //변환
    pos = mul(mWVP, pos);
    
    o.pos = pos;
    
    float4 LitColor = CalculateLight(posv, nrm, mWV, mView, mInverseTranseposeWV);
    o.col = LitColor;
    //float4 N = mul(mInverseTranseposeWorld, nrm);
    //N.w = 0;
    //N = normalize(N);
    //float4 DebugNormalColor = float4(N.xyz * 0.5f + 0.5f, 1.0f);

    //o.col = DebugNormalColor + LitColor * 0.0001f;
    
    
    
    o.uv = uv;
    
    return o;
}
