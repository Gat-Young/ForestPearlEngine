//
// FX 파티클 VertexShader : 빌보드 (연기 · 총구 섬광)   - Cena Particle.fx 를 엔진 방식으로
//
//  판 메쉬(Plane_StaticMesh) 의 정점 위치는 안 쓰고 UV 로 네 모서리를 만듦 → 메쉬 방향 무관
//  컴포넌트 월드 위치 = 판 가운데
//
//  Axis.w = 0 : 보통 빌보드  (화면에 붙은 판, Size.xy = 가로 · 세로, Size.z = 화면 안 회전)
//  Axis.w = 1 : 늘인 빌보드  (판의 u 축을 Axis.xyz 에 고정, v 축만 카메라 쪽으로 돌림, Size.x = 길이, Size.y = 폭)
//
// ★ 엔진 행렬은 C++ 행우선 그대로 올라옴 → HLSL 에서는 mul(행렬, 벡터) 순서 (DefaultVertexShader 와 같음)

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

//머티리얼 JSON 의 "FXVertexCB" 와 이름이 같아야 엔진이 관리함 (b0 Object, b1 ViewPort, b2 Light 다음)
cbuffer FXVertexCB : register(b3)
{
    float4 Axis;    //xyz = 늘인 축 (월드, 정규화),  w = 0 보통 빌보드 / 1 늘인 빌보드
    float4 Size;    //x = 가로(길이), y = 세로(폭), z = 화면 안 회전 (라디안)
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

//입력은 "Default" 입력 레이아웃과 같게 받아 둠 (안 쓰는 것도 선언)
VSOutput VS_Main(
                float4 pos : POSITION,
                float4 nrm : NORMAL,
                float4 col : COLOR0,
                float2 uv  : TEXCOORD0,
                float4 T   : TANGENT
                )
{
    VSOutput o = (VSOutput) 0;
    float2 q = float2(uv.x - 0.5f, 0.5f - uv.y);           //판 모서리 : -0.5 ~ 0.5

    if (Axis.w < 0.5f)
    {
        //보통 빌보드 : 뷰 공간에서 가운데 + 모서리 → 늘 카메라를 봄 (= 월드 S·Rz·(뷰 회전)⁻¹·T 와 같은 결과)
        float s, c;
        sincos(Size.z, s, c);
        q *= Size.xy;
        q = float2(q.x * c - q.y * s, q.x * s + q.y * c);   //화면 안 회전 : 저마다 다르게 돌려 같은 그림 티 안 나게

        float4 Pv = mul(mWV, float4(0, 0, 0, 1));
        Pv.xy += q;
        o.pos = mul(mProj, Pv);
    }
    else
    {
        //늘인 빌보드 : r = 축, s = normalize(r × 카메라쪽)  (Cena PtclDrawAxis)
        float3 C   = mul(mWorld, float4(0, 0, 0, 1)).xyz;           //판 가운데 (월드)
        float3 Eye = mul(mInverseView, float4(0, 0, 0, 1)).xyz;     //뷰 역행렬의 원점 = 카메라 위치
        float3 r   = Axis.xyz;
        float3 s   = cross(r, normalize(Eye - C));
        float  len = length(s);
        s = (len > 0.001f) ? s / len : float3(0, 1, 0);             //카메라가 축 방향 그대로 봄 : 아무 쪽

        float3 P = C + r * (q.x * Size.x) + s * (q.y * Size.y);
        o.pos = mul(mProj, mul(mView, float4(P, 1)));
    }

    o.uv = uv;
    return o;
}
