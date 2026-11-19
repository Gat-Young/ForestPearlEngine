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
    float3 normal : NORMAL;
    float clip : SV_ClipDistance0;
};

VSOutput VS_Main(float4 position : POSITION, float3 normal : NORMAL,
    float4 color : COLOR0, float2 uv : TEXCOORD0, float4 tangent : TANGENT)
{
    VSOutput output;
    output.position = mul(mWVP, position);
    output.normal = mul((float3x3)mInverseTransposeWorld, normal);
    // This demo's horizontal water is y=0. Keep only above-water model geometry.
    // Clipping stays in the project shader; no engine clip-plane or pass changes.
    output.clip = mul(mWorld, position).y;
    return output;
}
