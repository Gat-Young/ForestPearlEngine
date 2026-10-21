//
// 기본 VertexShader 소스

//상수 버퍼
cbuffer ObejctConstBuffer : register(b0)
{
    matrix mWorld;
    matrix mView;
    matrix mProj;
    matrix mWV;
    matrix mWVP;
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
    //pos = mul(pos, mWVP);
    
    //matrix m = mWorld * mView * mProj;
    //pos = mul(pos, mWorld);
    //pos = mul(pos, m);
    pos = mul(mWVP, pos);
    //pos = mul(pos, mWVP);
    
    //pos = mul(pos, m);
    o.pos = pos;
    o.col = col;
    
    return o;
}
