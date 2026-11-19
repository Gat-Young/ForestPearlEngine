// b0 layout is the engine's unchanged 1280-byte MVP buffer (column-major HLSL).
cbuffer ObjectConstBuffer : register(b0)
{
    matrix mWorld, mInverseWorld, mTransposeWorld, mInverseTransposeWorld;
    matrix mView, mInverseView, mTransposeView, mInverseTransposeView;
    matrix mProj, mInverseProj, mTransposeProj, mInverseTransposeProj;
    matrix mWV, mInverseWV, mTransposeWV, mInverseTransposeWV;
    matrix mWVP, mInverseWVP, mTransposeWVP, mInverseTransposeWVP;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 projected : TEXCOORD1;
    float3 toEye : TEXCOORD2;
};

VSOutput VS_Main(float4 position : POSITION, float3 normal : NORMAL,
    float4 color : COLOR0, float2 uv : TEXCOORD0, float4 tangent : TANGENT)
{
    VSOutput output;
    output.position = mul(mWVP, position);
    output.uv = uv;
    // On the horizontal reflection plane, reflected and original projected positions match.
    // Using clip coordinates also works with a fixed-size RT and a resized/letterboxed window.
    output.projected = output.position;
    float3 worldPosition = mul(mWorld, position).xyz;
    float3 eyePosition = mul(mInverseView, float4(0, 0, 0, 1)).xyz;
    output.toEye = eyePosition - worldPosition;
    return output;
}
