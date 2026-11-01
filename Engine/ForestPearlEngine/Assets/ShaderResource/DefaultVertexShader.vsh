//
// 기본 VertexShader 소스

// Vertex Light 운용 Shader
// b2 : Light,  b3 : Material
//#include "VertexLight.hlsli"

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
    float4 pos  : SV_POSITION;
    float4 col  : COLOR0;
    float4 nrm  : TEXCOORD2;
    float4 posv : TEXCOORD1;
    float2 uv   : TEXCOORD0;
};
 
 

////////////////////////////////////////////////////////////////////////////// 
//
//! Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

VSOutput VS_Main(
                float4 pos : POSITION,       //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
                float4 nrm : NORMAL,         //[입력] 노멀 normal
                float4 col : COLOR0,         //[입력] 정점색
                float2 uv : TEXCOORD0        //[입력] UV
                )
{
    //입력된 정보 그대로 출력..
    VSOutput o = (VSOutput) 0;

    //변환
    o.pos = mul(mWVP, pos);
    o.col = col;
    
       
    //float4 LitColor = CalculateLight(posv, nrm, mWV, mView, mInverseTranseposeWV);
    
    float4 N = mul(mInverseTranseposeWV, nrm);
    N.w = 0;
    o.nrm = normalize(N);
    
    //View 변환
    float4 posv = mul(mWV, pos);
    o.posv = posv;
    
    
    o.uv = uv;
    
    return o;
}
