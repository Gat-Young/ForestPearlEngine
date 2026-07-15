//
// Demo.fx : 기본 셰이더 소스.

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
                float4 pos : POSITION,      //[입력] 정점좌표. Vertex Position(Model Space, 3D) 
                float4 col : COLOR0         //[입력] 정점색. Vertex Color : "Diffuse"
                )
{
    //입력된 정보 그대로 출력..
    VSOutput o = (VSOutput) 0;
    o.pos = pos;
    o.col = col;
    
    return o;
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
	//지정색 출력.
	//float4 col = {1, 0, 1, 1};

    return col;
}


/**************** end of file "Demo.fx" ***********************/
