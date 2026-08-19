//VertexShader

#include "funcs.fx"

//상수 버퍼
cbuffer ObejctConstBuffer : register(b0)
{
    matrix mWorld;
    matrix mView;
    matrix mProj;
    matrix mWVP;
};

cbuffer VertexConstBuffer : register(b1)
{
    float4 VSColor;
    float VSPer;
    float VSx;
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

VSOutput
        VS_Main(
                float4 pos : POSITION, //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
                float4 col : COLOR0 //[입력] 정점색. Vertex Color : "Diffuse"
                )
{
    VSOutput o = (VSOutput) 0;
    
    //변환
    //pos.x += VSx;
  
    
    //색상 변환
    col = col + RGBGen(VSPer, 0);
    
    o.pos = pos;
    o.col = col;
    
    return o;
}