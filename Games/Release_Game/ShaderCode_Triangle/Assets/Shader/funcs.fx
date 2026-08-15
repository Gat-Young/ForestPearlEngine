
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


//색상 변화 함수 : 선형 보간

float4 RGBGen(float a, uint frm)
{
    //지정 색상 처리
    float4 c = GPU_RGB[frm];
    
    //색상 농도 조절
    c = c * a;
    
    return c;
}
