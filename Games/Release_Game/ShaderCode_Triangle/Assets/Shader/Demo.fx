//
// Demo.fx : 기본 셰이더 소스.

//상수 버퍼
cbuffer ConstBuffer : register(b0)
{
    matrix mWorld;
    matrix mView;
    matrix mProj;
    matrix mWVP;
};

cbuffer ConstBuffer : register(b1)
{
    float4 Color;
    float fTrans;
};

//VS 출력 구조체
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
};

//셰이더 사용자 정의 함수 선언

//이동 변환
float4 transform(float4 pos);

//회전 변환 (z축 기준)
float4 rotationZ(float4 pos, float degree);

//스케일 변환
float4 scaling(float4 pos, float s);


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
    pos.w = 1.0f;
    
    //test1 이동 변환
    pos = transform(pos);
    
    //test2 회전 변환
    //pos = rotationZ(pos, 45);
    
    //test3 회전 후 이동
    //pos = rotationZ(pos, 45);
    //pos = transform(pos);
    
    //test4 이동 후 회전
    //pos = transform(pos);
    //pos = rotationZ(pos, 45);
    
    //test5 크기 변환 50% 축소
    //pos = scaling(pos, 0.5f);
    
    //test6 크기 변환 200% 확대
    //pos = scaling(pos, 2.0f);
    
    //test7 정점색 지정
    //col = float4(1, 1, 0, 1);
    
    //test8 정점 좌표를 색상으로 지정
    //col = pos;
    
    //변환
    pos = mul(pos, mWVP);
    
    o.pos = pos;
    o.col = col;
    
    return o;
}


//셰이더 사용자 함수 정의

//이동 변환
float4 transform(float4 pos)
{
    pos.x += 0.5f;
    
    return pos;
}

//회전 변환 (z축 기준)
float4 rotationZ(float4 pos, float degree)
{
    float r = radians(degree);
    
    float4 vp = pos;
    vp.x = pos.x * cos(r) - pos.y * sin(r);
    vp.y = pos.x * sin(r) + pos.y * cos(r);
    
    return vp;
}

//스케일 변환
float4 scaling(float4 pos, float s)
{
    float4 vp = pos;
    
    vp *= s;
    vp.w = pos.w;
    
    return vp;
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
    
    //test1 외부-렌더링 파이프라인에서 공급된 색상을 그대로 출력
    float4 color = col;

    //test2  지정색 출력
    //color = float4(1, 1, 0, 1);
    
    //test3 색상 혼합 테스트 : 아래의 코드를 하나씩 테스트
    //color = col + float4(1, 0, 0, 1);
    //color = col - float4(1, 0, 0, 1);
    //color = col * 0.5f;
    //color = col + 0.5f;
    //color = col + float4(1, 1, 1, 1);
    //color = col - 1;
    
    //test4 색상 반전
    //color = 1 - col;
    
    //test5 좌표를 색상으로 출력
    //color = pos;
    
    //test6 픽셀 버리기
    //if (color.r < 0.5f) clip(-1);
    //if (pos.x > 600) clip(-1);
    
    //색상 필터
    //test1
    //float4 color = { col.r, 0, 0, 1};
    
    //test2
    //float4 color = col * float4(1, 0, 0, 1);
    
    //test3
    //float4 color = col + float4(0, -1, -1, -1);
    //float4 color = col - float4(0, 1, 1, 1);
    
    //test4
    //float4 color = col * float4(1, 0, 0, 1);
    //float4 color = col * float4(0, 1, 0, 1);
    //float4 color = col * float4(0, 0, 1, 1);
    
    //test5
    //float4 color = col * float4(1, 0, 0, 1) + col * float4(0, 1, 0, 1);
    
    //test6
    //float4 color = col * float4(0, 0, 0, 1);
    
    //test7
    //float4 color = col.r;
    //float4 color = float4(col.r, col.r, col.r, col.r);
    //float4 color = col.g;
    //float4 color = col.b;
    
    //test8
    //float4 color = col.r * 0.2126 + col.g * 0.7152 + col.b * 0.0722;
    
    //color.a = 1;
    
    return color;
}


/**************** end of file "Demo.fx" ***********************/
