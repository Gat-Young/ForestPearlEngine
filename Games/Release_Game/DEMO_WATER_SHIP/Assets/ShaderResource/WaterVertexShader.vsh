//
// Water VertexShader
// Uses the same b0/b13 layout as DefaultPlanarReflectionVertexShader, and passes
// ReflectionClip using the ReflectionViewProj computed by FPPlanarReflectionComponent.

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

//Planar Reflection Clip Plane / ReflectionViewProj constant buffer (fixed slot b13)
cbuffer ClipPlanarReflectionConstantBuffer : register(b13)
{
    float4 GlobalClipPlane;
    uint ClipPlaneEnabled;
    float3 Padding;
    matrix ReflectionViewProj;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 ReflectionClip : TEXCOORD4;
    float ClipDistance : SV_ClipDistance0;
};

VSOutput VS_Main(
                float4 pos : POSITION,
                float4 nrm : NORMAL,
                float4 col : COLOR0,
                float2 uv : TEXCOORD0,
                float4 T : TANGENT
                )
{
    VSOutput o = (VSOutput) 0;

    o.pos = mul(mWVP, pos);
    o.uv = uv;

    float4 WorldPos = mul(mWorld, pos);

    //Default: no clipping
    o.ClipDistance = 1.0f;
    if (ClipPlaneEnabled != 0)
    {
        o.ClipDistance = dot(WorldPos, GlobalClipPlane);
    }

    //Clip-space position for sampling the reflection RenderTarget, using the
    //reflection camera's View*Projection (left un-divided; the divide happens
    //per-pixel in the pixel shader to avoid stretching).
    o.ReflectionClip = mul(ReflectionViewProj, WorldPos);

    return o;
}
