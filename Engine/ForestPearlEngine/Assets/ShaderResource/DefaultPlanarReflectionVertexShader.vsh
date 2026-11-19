//
// Default Planar Reflection VertexShader 소스
// DefaultVertexShader와 동일하지만, ClipPlanarReflectionConstantBuffer(b13)에 담긴
// 반사 카메라의 View*Projection으로 ReflectionUV를 계산해 PS로 넘겨줌
// (스크린 스페이스 반사 UV 매핑. 계산 방식은 LakePixelShader.psh 참고)

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

//Planar Reflection Clip Plane / ReflectionViewProj 상수 버퍼 (고정 슬롯 b13)
cbuffer ClipPlanarReflectionConstantBuffer : register(b13)
{
    float4 GlobalClipPlane;
    uint ClipPlaneEnabled;
    float3 Padding;
    matrix ReflectionViewProj;
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
    float2 ReflectionUV : TEXCOORD4;
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

    float4 WorldPos = mul(mWorld, pos);

    //기본값: 클리핑하지 않음
    o.ClipDistance = 1.0f;

    //Clip Plane이 활성화된 경우
    if (ClipPlaneEnabled != 0)
    {
        o.ClipDistance = dot(WorldPos, GlobalClipPlane);
    }

    //반사 카메라의 View*Projection으로 반사 RenderTarget 샘플링용 UV 계산
    float4 ReflectedClip = mul(ReflectionViewProj, WorldPos);
    float2 NDC = ReflectedClip.xy / ReflectedClip.w;
    o.ReflectionUV = float2(
        NDC.x * 0.5f + 0.5f,
        0.5f - NDC.y * 0.5f
    );

    return o;
}
