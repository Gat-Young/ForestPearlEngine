//
// SkyBox Quad 운용 셰이더
//
//
//
// <지원 기능>
// 1. Clip공간 Quad에 대한 정점 변환

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

cbuffer ClipPlanarReflectionConstantBuffer : register(b13)
{
    float4 GlobalClipPlane;
    uint ClipPlaneEnabled;
    float3 Padding;
};


//VS 출력 구조체
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float4 posv : TEXCOORD1;
    float2 uv : TEXCOORD0;
    float4 T : TEXCOORD2;
    float4 N : TEXCOORD3;
    float3 uvw : TEXCOORD4;
    float ClipDistance : SV_ClipDistance0;
};

////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main : 정점 셰이더 메인 함수
//
////////////////////////////////////////////////////////////////////////////// 

//혹시 모르니 모든 정보를 보냄(나중에 광원 받으면 영향을 줄 수 있도록)
VSOutput VS_Main(
                float4 pos : POSITION, //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
                float4 nrm : NORMAL, //[입력] 노멀 normal
                float4 col : COLOR0, //[입력] 정점색
                float2 uv : TEXCOORD0, //[입력] UV
                float4 T : TANGENT //[입력] 탄젠트 노멀 Tangent Normal
                ) 
{
    float4 VSpos = pos;
    float4 VScol = col;
    float4 VSposv = mul(mWV, pos);
    float2 VSuv = uv;
    float4 VST = T;
    float4 VSN = nrm;
    float3 WSuvw = 0;

    float4 ViewPos = mul(mInverseProj, VSpos);
    ViewPos.xyz /= ViewPos.w;
    
    float3 WorldDir = mul(mInverseView, float4(ViewPos.xyz, 0.0f)).xyz;
    
    WSuvw = WorldDir;
    
    //입력된 정보 그대로 출력..
    VSOutput o = (VSOutput) 0;
    o.pos = VSpos;

    o.col = VScol;

    o.posv = VSposv;

    o.uv = VSuv;

    o.T = VST;

    o.N = VSN;
    
    o.uvw = WSuvw;

    o.ClipDistance = 1.0f;
    
    return o;
}
