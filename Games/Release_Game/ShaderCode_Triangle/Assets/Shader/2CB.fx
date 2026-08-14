//
// Demo.fx : 기본 셰이더 소스.

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

cbuffer PixelConstBuffer : register(b2)
{
    float4 PSColor;
    float PSPer;
};

//VS 출력 구조체
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
};

//GPU 내부 데이터
static float4 GPU_RGB[] =
    {
        { 1, 0, 0, 1 },
        { 0, 1, 0, 1 },
        { 0, 0, 1, 1 },
        { 1, 0, 0, 1 },
    };

//색상 함수 : 선형보간
float4 RGBGen(float a, uint frm);

////////////////////////////////////////////////////////////////////////////// 
//
//! Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

VSOutput
        VS_Main(
                float4 pos : POSITION,      //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
                float4 col : COLOR0         //[입력] 정점색. Vertex Color : "Diffuse"
                )
{
    VSOutput o = (VSOutput) 0;
    
    //변환
    pos.x += VSx;
    
    //색상 변환
    col = col + RGBGen(VSPer, 0);
    
    o.pos = pos;
    o.col = col;
    
    return o;
}

//색상 변화 함수 : 선형 보간

float4 RGBGen(float a, uint frm)
{
    //지정 색상 처리
    float4 c = GPU_RGB[frm];
    
    //색상 농도 조절
    c = c * a;
    
    return c;
}


////////////////////////////////////////////////////////////////////////////// 
//
//! Pixel Shader Main : 픽셀 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

float4 PS_Main(
                float4 pos : SV_POSITION, //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
                float4 col : COLOR0 //[입력] 정점색. Vertex Color : "Diffuse" 
                ) : SV_TARGET
{
    
    float4 color = 1;
    
    color = col + PSColor * PSPer;
    
    color *= PSPer;
	
    return color;
    
    //return col;
}


/**************** end of file "Demo.fx" ***********************/
