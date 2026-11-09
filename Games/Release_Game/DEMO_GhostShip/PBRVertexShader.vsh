//
// 기본 VertexShader 소스

// Vertex Light 운용 Shader
// b2 : Light,  b3 : Material
//#include "VertexLight.hlsli"

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

cbuffer ViewPortConstBuffer : register(b1)
{

};

//VS 출력 구조체
struct VSOutput
{
    //float4 pos  : SV_POSITION;
    //float4 col  : COLOR0;
    //float4 posv : TEXCOORD1;
    //float2 uv   : TEXCOORD0;
    //float4 T    : TEXCOORD2;
    //float4 N    : TEXCOORD3;
    
    float4 pos : SV_POSITION;
    //float4 diff : COLOR0;
	//float4 spec : COLOR1;
    float2 uv : TEXCOORD0;
    float3 uvw : TEXCOORD1; //큐브멥용, 3성분 필요(u,v,w)
    float4 posv : TEXCOORD2;
    float4 nrm : TEXCOORD3;
};
 
 

////////////////////////////////////////////////////////////////////////////// 
//
//! Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

//VSOutput VS_Main(
//                float4 pos : POSITION,       //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
//                float4 nrm : NORMAL,         //[입력] 노멀 normal
//                float4 col : COLOR0,         //[입력] 정점색
//                float2 uv : TEXCOORD0,       //[입력] UV
//                float4 T : TANGENT           //[입력] 탄젠트 노멀 Tangent Normal
//                )
//{
//    float4 VSpos = pos;
//    float4 VScol = col;
//    float4 VSposv = mul(mWV, pos);
//    float2 VSuv = uv;
//    float4 VST = T;
//    float4 VSN = nrm;

//    변환
//    VSpos = mul(mWVP, pos);
    
       
//    float4 LitColor = CalculateLight(posv, nrm, mWV, mView, mInverseTranseposeWV);
    
    
//    View Space로 변환
    
//    float4 N = float4(nrm.xyz, 0.0f);
//    N = mul(mInverseTranseposeWV, N);
//    VSN = float4(normalize(N.xyz), 0.0f);
    
//    float TangentSign = T.w;
//    float4 TangentVector = float4(T.xyz, 0.0f);
//    float3 Tangent = mul(mWV, TangentVector);
//    Tangent = normalize(Tangent);
    
//    Gram - Schmudt 직교화
//    Tangent = Tangent - VSN.xyz * dot(VSN.xyz, Tangent);
//    Tangent = normalize(Tangent);
    
//    float TagentSign = T.w;
    
//    음수 행렬식에 따른 방향 반전 보전 (Mirror 대응)
//    if(determinant((float3x3)mWV) < 0.0f)
//    {
//        TangentSign = -TangentSign;
//    }
//    VST = float4(Tangent, TangentSign);
    
//    입력된 정보 그대로 출력..
//    VSOutput o = (VSOutput) 0;
//    o.pos = VSpos;

//    o.col = VScol;

//    o.posv = VSposv;

//    o.uv = VSuv;

//    o.T = VST;

//    o.N = VSN;
    
//    return o;
//}



////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 


// 인풋레이아웃 바꾸는법
VSOutput VS_Main(
				  float4 pos : POSITION, //[입력] 정점좌표. Vertex Position (Model Space, 3D)
    			  float4 nrm : NORMAL, //[입력] 노멀 normal 
                  float4 col : COLOR0, //[입력] 정점색
                  float2 uv : TEXCOORD0 //[입력] 텍스처 좌표 Texture Coordiates.
				)
{


    //--------------------------
    // 정점 변환
    //--------------------------
    pos.w = 1;
    //float4 posw = mul(pos, mTM);        //pos = pos * mTM
    //float4 posv = mul(pos, mView);
    float4 posv = mul(pos, mWV);
    float4 posh = mul(posv, mProj);
     
    
    //--------------------------
    // 조명 계산
    //--------------------------
    //...PPL 처리.
    
    //--------------------------
	// 환경멥 계산.: Cube Map  
    //--------------------------
    float3 eR = EnvMapCube(pos, nrm, vEyePos, mW);


    //--------------------------
    // 정보 출력.
    //--------------------------
    VSOutput o = (VSOutput) 0;
    o.pos = posh;
    //o.diff = diff;
	//o.spec = spec;
    o.uv = uv;
    o.uvw = eR; //환경광 멥핑 좌표 리턴.
    o.posv = posv;
    o.nrm = nrm;

    return o;
}