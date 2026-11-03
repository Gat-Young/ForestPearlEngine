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
    float4 posv : TEXCOORD1;
    float2 uv   : TEXCOORD0;
    float4 T    : TEXCOORD2;
    float4 N    : TEXCOORD3;
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
                float2 uv : TEXCOORD0,       //[입력] UV
                float4 T : TANGENT           //[입력] 탄젠트 노멀 Tangent Normal
                )
{
    float4 VSpos = pos;
    float4 VScol = col;
    float4 VSposv = mul(mWV, pos);
    float2 VSuv = uv;
    float4 VST = T;
    float4 VSN = nrm;

    //변환
    VSpos = mul(mWVP, pos);
    
       
    //float4 LitColor = CalculateLight(posv, nrm, mWV, mView, mInverseTranseposeWV);
    
    
    //View Space로 변환
    
    float4 N = float4(nrm.xyz, 0.0f);
    N = mul(mInverseTranseposeWV, N);
    VSN = float4(normalize(N.xyz), 0.0f);
    
    float TangentSign = T.w;
    float4 TangentVector = float4(T.xyz, 0.0f);
    float3 Tangent = mul(mWV, TangentVector);
    Tangent = normalize(Tangent);
    
    //Gram - Schmudt 직교화
    Tangent = Tangent - VSN.xyz * dot(VSN.xyz, Tangent);
    Tangent = normalize(Tangent);
    
    float TagentSign = T.w;
    
    //음수 행렬식에 따른 방향 반전 보전 (Mirror 대응)
    if(determinant((float3x3)mWV) < 0.0f)
    {
        TangentSign = -TangentSign;
    }
    VST = float4(Tangent, TangentSign);
    
    //입력된 정보 그대로 출력..
    VSOutput o = (VSOutput) 0;
    o.pos = VSpos;

    o.col = VScol;

    o.posv = VSposv;

    o.uv = VSuv;

    o.T = VST;

    o.N = VSN;
    
    return o;
}
