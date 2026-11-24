//
// 기본 VertexShader 소스

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

//Planar Reflection Clip Plane 상수 버퍼 (고정 슬롯 b13)
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
    float ClipDistance : SV_ClipDistance0;
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
                float2 uv : TEXCOORD0,        //[입력] UV
                float4 T : TANGENT //[입력] 탄젠트 노멀 Tangent Normal
                )
{
    //입력된 정보 그대로 출력..
    VSOutput o = (VSOutput) 0;

    //Clip Plane 판정은 World Space 기준이므로 pos가 Clip Space로 덮어써지기 전에 계산
    float4 WorldPos = mul(mWorld, pos);

    //변환
    //pos = mul(pos, mWVP);

    //matrix m = mWorld * mView * mProj;
    //pos = mul(pos, mWorld);
    //pos = mul(pos, m);
    pos = mul(mWVP, pos);
    //pos = mul(pos, mWVP);
    
    //pos = mul(pos, m);
    o.pos = pos;
    o.col = col;

    //기본값: 클리핑하지 않음
    o.ClipDistance = 1.0f;

    //Clip Plane이 활성화된 경우
    if (ClipPlaneEnabled != 0)
    {
        o.ClipDistance = dot(WorldPos, GlobalClipPlane);
    }

    return o;
}
