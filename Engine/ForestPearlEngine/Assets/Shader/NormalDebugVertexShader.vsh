//
// Normal Draw용 Shader

#include "NormalCalculate.hlsli"

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


cbuffer NormalDebugConstBuffer : register(b2)
{
    matrix mNormal;
    float NormalLength;
};

// Mesh Vertex Input
struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    // 0 : Start
    // 1 : End
    uint VertexID : SV_VertexID;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
};

// Vertex Shader
VSOutput VS_Main(VSInput input)
{
    VSOutput Output = (VSOutput) 0;

    // Local Position -> World Position
    float3 WorldPosition = mul( mWorld, float4(input.Position, 1.0f) ).xyz;

    // Local Normal -> World Normal
    // CPU에서 계산한 역전치 Normal Matrix 사용
    float3 WorldNormal = TransformWorldNormal(input.Normal, mNormal);

    // Normal 정규화
    // 길이가 0인 비정상 Normal은 퇴화된 선으로 처리
    float LengthSq = dot(WorldNormal, WorldNormal);

    WorldNormal =
        LengthSq > 1e-12f
        ? WorldNormal * rsqrt(LengthSq)
        : float3(0.0f, 0.0f, 0.0f);

    // VertexID에 따라 Line Start / End 생성
    if (input.VertexID == 1)
    {
        WorldPosition += WorldNormal * NormalLength;
    }

    // World -> View -> Projection
    Output.Position = mul(
        mProj,
        mul(mView, float4(WorldPosition, 1.0f))
    );

    // Green
    Output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f);

    return Output;
}