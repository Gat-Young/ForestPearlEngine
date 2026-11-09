//////////////////////////////////
//
// Per Pixel Light 운용 셰이더 코드
//
//

#ifndef PIXEL_LIGHT_HLSLI
#define PIXEL_LIGHT_HLSLI

//Light 계산 함수
#include "LambertLight.hlsli"
#include "SpecularBlinPhong.hlsli"

//광원 종류
#define 	LightType_Direction 0
#define 	LightType_Point     1
#define 	LightType_Spot      2
#define 	LightType_Rect      3


//광원 최대 개수
#define     Light_MAX           64


#define     PI 3.141592f

struct LIGHT
{
    uint LightType;
    float3 Padding;
    float4 LightPosition;
    float4 Direction;
    float Range;
    float3 Padding_2;
    float4 Diffuse;
    float4 Ambient;
    float4 Specular;
    float4 Attenuation;
    float4 LightColor;
    //float4 Strength;
};

// 상수 버퍼크기 9232B
cbuffer LightConstBuffer : register(b2)
{
    uint LightCnt; //바인딩된 Light 개수
    float3 Padding;
    
    LIGHT Light[Light_MAX]; //Light를 최대 개수만큼 배열 생성 144B * 64 = 9216B
};

struct MATERIAL
{
    float4 BaseColor;
    float4 Diffuse;
    float4 Ambient;
    float4 Specular;
    float4 Roughness;
    float4 Emissive;
};

cbuffer MaterialConstBuffer : register(b3)
{
    MATERIAL Material;
};

//조명 처리 함수.
void CalculateLight(float4 posv, float4 nrm, matrix mView, out float4 OutDiffuse, out float4 OutSpecular, out float4 OutEmissive);

//조명 계산 함수
void DirLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular);
void PointLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular);
void SpotLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular);
void RectLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular);



//////////////////////////////////////////////////////////
// PBR
float4 Diffuse_BRDF(float4 nrm, matrix mWV, matrix mView, float4 base, float4 metal, float4 ao);
float4 Specular_BRDF(float4 posv, float4 nrm, matrix mWV, matrix mView, float4 base, float4 metal, float4 rough);


// DFG 함수

// D
float4 NDF(float4 N, float4 H, float4 smooth, float ka);


// F
float4 SchlickFresnel(float4 R0, float4 V, float4 H);
float4 SchlickFresnel3(float4 V, float4 H, float4 base, float4 metal);

// G
float4 G1_SB(float4 N, float4 V, float4 k);
float4 GAF_GGX(float4 L, float4 V, float4 N, float4 H, float4 smooth, float4 ka);
float G1_Smith(float4 N, float4 V, float ka);
float4 GAF_GGX2(float4 L, float4 V, float4 N, float4 H, float4 smooth, float ka);


//! GAF : Smith-GGX
//! 
float G1_Smith(float4 N, float4 V, float ka)
{
    float a2 = pow(ka, 2);
    float n_v = dot(N, V);
    float sq = sqrt(a2 + (1 - a2) * pow(n_v, 2));
    
    float g = (2 * (n_v)) / (n_v + sq);
    return g;
    
}


//! GAF : Smith-GGX
//! 미세면 기하감쇠 함수 (G) : Smith-GGX 모델, 거칠기(Rough map 참조)
//! 
float4 GAF_GGX2(float4 L, float4 V, float4 N, float4 H, float4 smooth, float ka)
{
    //float a = 1;
    //float a = 64;
    //float a = rough.r;    
    //float a = rough.r * 4;
    //float a = rough.r * 64;                           //보통 거침, 광택, rough 값으로 조절.(rough 값으로 반사 저하됨, 이전 데모와 유사한 광량확보를 위해 2배 보정 : 128 -> 256)★
    //float a = rough.r * 128;                           //보통 거침, 광택, rough 값으로 조절.(rough 값으로 반사 저하됨, 이전 데모와 유사한 광량확보를 위해 2배 보정 : 128 -> 256)★
    //a = a* a *a * a;
    float a = 1 - smooth;
    
    float4 G1L = G1_Smith(N, L, a);
    float4 G1V = G1_Smith(N, V, a);
    float G = G1L * G1V;
    
    return G;
}


//! G1 : Schlick-Beckmann
//! 
float4 G1_SB(float4 N, float4 V, float4 k)
{
    //float k = ka * sqrt(2/PI);
    float n_v = dot(N, V);
    //float n_v = max(dot(N, V), 0);

    //float c = dot(N, V) / (ka * sqrt(1-pow(dot(N, V), 2)));
    float g = n_v / (n_v * (1 - k) + k);

    return g;
}



//! GAF : Schlick GGX, (UE4)
//! 
float4 GAF_GGX(float4 L, float4 V, float4 N, float4 H, float4 smooth, float4 ka)
{
    float a = 1 - smooth;
    float k = a / 2;
    
    float G1L = G1_SB(N, L, k);
    float G1V = G1_SB(N, V, k);
    float G = G1L * G1V;
    
    return G;
}


float4 NDF(float4 N, float4 H, float4 smooth, float ka)
{
    //ND 편차 <테스트>
    //float a = 1;                                     //미세면 노말 연산 없음, 기본값 처리됨. "수퍼 골드"
    //float a = 64;                                    //보통 거침, 광택.
    //float a = 128;                                   //보통 매끈, 더 광택.
    //float a = 512;                                   //아주 매끈, 더 고광택.
    //float a = smooth.r * 1;                          //100% 거침, 광택 0
    //float a = smooth.r * 8;                          //아주 거침, 광택 극소, 미량.
    //float a = smooth.r * 16;                         //거침, 광택 적음,
    //float a = smooth.r * 32;                         //덜 거침, 광택 적음, 더 부드러운 광택 
    //float a = smooth.r * 64;                         //약간 거침, 광택 있음, 부드러운 광택, 매끈 없음. 
    //float a = smooth.r * 128;                        //보통 거침, 광택, 금속 광택, (기준)
    //float a = smooth.r * 256;                        //약한 거침, 광택x2, 더 금속 광택,    
    //float a = smooth.r * 512;                        //매끈, 더 광택, 더 강한 금속 광택.
    //float a = smooth.r * 1024;                       //아주 매끈, 고 광택, 피막 광택.
    
    float a = smooth.r * ka; //ND 분표도, 광택량, rough 값으로 조절.    
    
    
    //미세면 노말 분포 함수 (D) : BlinnPhong 사용.
    //float4 D = 1;
    float4 D = ((a + 2) / (2 * PI)) * pow(dot(N, H), a); //BlinnPhong NDF 기본.     
    //float4 D = ((a+4)/(4*PI))*pow(dot(N,H), a);      //BlinnPhong NDF 기본 x2
    //float4 D = ((a+8)/8)*pow(dot(N,H), a);           //BlinnPhong NDF 기본 x4 
    
    return D;
}
float4 SchlickFresnel(float4 R0, float4 V, float4 H)
{
    //float f0 = 1.0f - saturate(max(dot(V, H), 0));
    //float f0 = 1.0f - max(dot(V, H), 0);
    float f0 = 1.0f - dot(V, H);
    //float f0 = 1.0f - saturate(dot(V, H));
    //float f0 = saturate(1.0f - dot(V, H));
    //float exp = pow(f0, 5);
    float4 Rf = R0 + (1 - R0) * pow(f0, 5); // Rf = R0 + (1-R0)(1-cos@)^5 
     
    return Rf;
}

float4 SchlickFresnel3(float4 V, float4 H, float4 base, float4 metal)
{
    
    //--------------------------------------------------------------
    // 프레넬 굴절율 (IOR, List of Relfraction) 
    // 참고1 : https://pixelandpoly.com/ior.html
    // 참고2 : https://mssiphiwemoyo.com/blog/what-is-index-of-refraction-ior/
    //--------------------------------------------------------------
    // <비금속>                     <금속> 
    // 공기(Air) : 1.0              금속 (iron) : 2.950
    // 물(Water) : 1.325            스틸 (Steel) : 2.500
    // 얼름(Ice) : 1.309            알루미늄(Aluminum) : 1.390
    // 유리(Glass) : 1.500          티타늄(Titanium) : 2.160 
    // 크리스탈(Crystal) : 2.000    금(Gold) : 0.470
    // 플라스틱(Plastic) : 1.460    은(Silver) : 0.18 (max 1.35)
    // 나일론(Nylon) : 1.530        동(Bronze) : 1.180
    // 자동차 페인트 : 1.175         구리(Copper) : 1.100  
    // 젤리(Jelly) : 1.33 ~ 1.35
    //
    // <표준 1.5 물체>
    // 흙(Soil) : 1.4 ~ 1.6          나무(Wood) : 1.50 ~ 1.70      
    // 가죽(Leather) : 1.48          종이(Paper0 : 1.50            
    // 대리석(Marble) : 1.5~1.7
    // 콘트리트(Concrete) : 1.5~1.7  아스팔트(Asphalt) : 1.635 (1.6~1.7)
    //
    // <보석>
    // 다이아몬드(Diamond) : 2.42   루비(Ruby) : 1.76
    // 사파이어(Sapphire) : 1.77    에메랄드(Emerald) : 1.58 ~ 1.60
    //--------------------------------------------------------------
    //float4 R0 = float4(0.02, 0.02f, 0.02, 0);         //Water
    //float4 R0 = float4(0.08, 0.08f, 0.08, 0);         //Glass
    //float4 R0 = float4(0.05, 0.05f, 0.05, 0);         //Plastic
    float4 R0 = float4(1.00, 0.71f, 0.29, 0); //Gold -> Fresnel 반사 테스트 추천. --> 텍스처기반 처리.
    //float4 R0 = float4(0.95, 0.93f, 0.88, 0);         //Silver
    //float4 R0 = float4(0.95, 0.64f, 0.54, 0);         //Copper
    //float4 R0 = float4(0.24, 0.24f, 0.24, 1);         //Iron
    //float4 R0 = float4(1, 0.0f, 0.0, 1);              //Iron (Red) 테스트용
    //float4 R0 = float4(0.0064, 0.0064, 0.0064, 0);    //자동차 페인트..(??)
    
    //반사율(광량,색상), 텍스처 기반 처리.
    //R0 = base;                                        //반사율(색) : Base(페인트) 기준
    //R0 = spec0;                                       //반사율(색) : metal 기준, Lit.Spec ~ Base(페인트) 보간
    //R0 = spec0 * R0;                                  //반사율(색) : metal 기준, Lit.Spec ~ Base(페인트) 보간 x (금색) 혼합
    //R0 = base * 0.3 + metal;                          //반사율(색) : Base(페인트) 30% + Metal 혼합
    //R0 = base * 0.3 + metal + R0*0.5f;                //반사율(색) : Base(페인트) 30% + Metal(금색 50%) 혼합1   
    //R0 = base * 0.3 + metal * R0;                     //반사율(색) : Base(페인트) 30% + Metal(금색) 혼합2 (이쁨!)  
    //R0 = base * 0.3 + (metal * R0)*0.7;               //반사율(색) : Base(페인트) 30% + Metal(금) 고정비 (3:7) 혼합 (이쁨!) ★ 
    //R0 = lerp(base*0.3f, R0 * metal, metal);          //반사율(색) : Base(페인트) 30% + Metal(금) 보간(3:x) 혼합 (이쁨!) ★  
    //R0 = lerp(base, R0, metal);                       //반사율(색) : Base(페인트) + Metal(금) 보간1 혼합 (이쁨2!)  
    //R0 = lerp(base, R0 * metal, metal);               //반사율(색) : Base(페인트) + Metal(금) 보간2 혼합 (이쁨3!) (이전 데모 연속성 유지)★  
    R0 = lerp(base * (1 - metal), R0 * metal, metal); //반사율(색) : Base(페인트) + Metal(금) 보간3 혼합 (이쁨4!) ★  
    //R0 = base * (1-metal) + R0 * metal;               //반사율(색) : Base(페인트) + Metal(금) 보간2 상동   
    
    //과포화 방지.
    R0 = saturate(R0);
    
    //float f0 = 1.0f - saturate(max(dot(V, H), 0));
    //float f0 = 1.0f - max(dot(V, H), 0);
    float f0 = 1.0f - dot(V, H);
    //float f0 = 1.0f - saturate(dot(V, H));
    //float f0 = saturate(1.0f - dot(V, H));
    //float exp = pow(f0, 5);
    float4 Rf = R0 + (1 - R0) * pow(f0, 5); // Rf = R0 + (1-R0)(1-cos@)^5 
     
    return Rf;
}



float4 Diffuse_BRDF(float4 nrm, matrix mWV, matrix mView, LIGHT LightData, float4 base, float4 metal, float4 ao)
{
    float4 N = nrm;
    N.w = 0;
    float4 L = LightData.Direction;
    L.w = 0;

    //뷰공간으로 정보를 변환.
    N = normalize(mul(N, mWV));
    L = normalize(mul(L, mView));
    
    //확산색 결정 : 비 Metallic 지역의 입자별 반사색 
    //float4 diff0 = base;                                          //<방식0> Base 일괄 선택 (NG, "페인트 벗겨진 금속 부위" 표현 불가)
    float4 diff0 = base - base * metal; //<방식1> 비금속지역 일괄 선택 
    //float4 diff0 = lerp(base,  0.08*(1-base * metal), metal);     //<방식2> 금속-비금속 지역 보간.
    
    //반사율 처리.
    //float4 albedo = base * ao;
    float4 albedo = diff0 * ao;
    
    //빛 강도 조절
    float4 lt = LightData.Diffuse * 30; // Light[0].Strength 
    //float4 lt =  diff0 * g_Lit.Strength;
    
    
    //조명 계산 : 태양광, 1광속, 등방 조건, diff = irradiance * (L.N) / π * albedo
    float4 diff = lt * max(dot(N, L), 0) / PI * albedo;
    
    //지역 간접광(Local illuminance) : IBL 적용 전까지는 간단하게 처리 합니다.
    float4 amb = LightData.Ambient * 30 * albedo; //주변광 강도 적용        //g_Lit.Strength
    //float4 amb = g_Lit.Ambient * ao;                          //주변광 강도 무시 (비추)
    //float4 amb = g_Lit.Ambient / PI * albedo;                 //주변광 조사량(irradiance) 적용    
        
	//포화도 처리 : 광량의 합을 1로 제한한다.
	//return saturate(diff + amb);
    return diff + amb;
    //return diff;
    
}

float4 Specular_BRDF(float4 posv, float4 nrm, matrix mWV, matrix mView, LIGHT LightData, float4 base, float4 metal, float4 rough)
{
    float4 P = posv;
    P.w = 0;
    float4 N = nrm;
    N.w = 0;
    float4 L = LightData.Direction;
    L.w = 0;

    //뷰공간으로 정보를 변환.
    N = normalize(mul(N, mWV));
    N.w = 0;
    L = normalize(mul(L, mView));
    L.w = 0;

    //시선백터 계산.
    float4 V = normalize(-P);
    V.w = 0;

    //하프벡터 계산.
    float4 H = normalize(L + V);
    H.w = 0;

    
    //반사광량 계산. (Metalic - 비 Metallic 지역의 반사광 (색상) 결정) 
    //float4 spec0 = g_Lit.Specular * metal;                        //<방식1> Diffuse 혼합시 1 초과 가능성, 과도한 반사, 에너지 보존법치 위배 가능성 있음. 비추.
    float4 spec0 = lerp(0.08 * LightData.Specular, base, metal); //<방식2> Metal - 비 Metal 지역 보간. 
    //float4 spec0 = lerp( 0.08 * 0.5f, base, metal.r);             //UE 엔진 기본값 spec = 0.5f
    
    //빛 강도 조절
    //float4 lt = g_Lit.Specular * g_Lit.Strength;
    float4 lt = spec0 * 30; //g_Lit.Strength
    
    //반사율 처리 : 입자별 거칠기. 
    float4 smooth = saturate(1 - rough);
    
   
    //---------------------
    // 조명 계산 <방식1> : BlinnPhong (레거시 라이팅) 
    //---------------------
    //float4 spec = lt * pow(max(dot(N, H), 0),  g_Mtrl.Power);         //재질 사용. 
    //return spec;
    
    //---------------------
    // 조명 계산 <방식2> : BlinnPhong + 반사율 조정(표면 거칠기, Roughness map 적용)
    //                     (데모용, smooth = 1 이면 레거시 specular 와 동일)
    //---------------------
    //float4 spec = lt * pow(max(dot(N, H), 0.0f), rough * 100.0f);     //rough 적용 
    //float4 spec = lt * pow(max(dot(N, H), 0.0f), smooth * 100.0f);    //smooth 적용 풀질비교
    //if(smooth.a) spec *= smooth;                                      //smooth 적용(2). 품질비교 요망.
    //return spec;
      
    //---------------------
    // 조명 계산 <방식3> : Cook-Torrance BRDF 모델 적용    
    //---------------------
    // 1. Frenel 반사율 적용 
    // 2. NDF 적용 (BlinnPhong NDF)
    // * 나머지는 기본값(1) 처리.
    //
    //float4 D = 1;
    //float4 D = NDF(N, H, smooth, 32);                         //덜 거침, 광택 적음, 더 부드러운 광택 
    //float4 D = NDF(N, H, smooth, 64);                         //약간 거침, 광택 있음, 부드러운 광택, 매끈 없음.
    float4 D = NDF(N, H, smooth, 128); //보통 거침, 광택, 금속 광택, rough 값으로 조절. (기준)
    //float4 D = NDF(N, H, smooth, 256);                        //약한 거침, 광택x2, 더 금속 광택, rough 값으로 조절.   
    //float4 D = NDF(N, H, smooth, 512);                        //매끈, 더 광택, 더 강한 금속 광택.
    //float4 D = NDF(N, H, smooth, 1024);                       //아주 매끈, 고 광택, 피막 광택.
    
    //float4 F = SchlickFresnel(R0, V, H);                      //프레넬 반사 (v1)
    float4 F = SchlickFresnel3(V, H, base, metal); //프레넬 반사 (v3)(R0 을 Base + Metalic 참조 혼합)(이쁨!)★
    
    //float4 G = GAF_v0(L, V, N, H);                            //미세면 기하 함수.(단축) ★
    //float4 G = GAF_CT(L, V, N, H);                            //미세면 기하 함수.(Cook-Torrance)★
    //float4 G = GAF_CT2(L, V, N, H);                           //미세면 기하 함수.(Cook-Torrance)(v2) 단축버전, "SIGGRAPH 2013" ★
    //float4 G = GAF(L, V, N, H, smooth, 128);                  //미세면 기하 함수.(Cook-Torrance)(v3)
    //float4 G = GAF_SB(L, V, N, H, smooth, 128);               //미세면 기하 함수.(Smith-Beckman) ★
    //float4 G = GAF_SSB(L, V, N, H, smooth, 128);              //미세면 기하 함수.(Schlick Smith-Beckman)★
    //float4 G = GAF_GGX(L, V, N, H, smooth, 128);              //미세면 기하 함수.(Schlick-GGX)★
    float4 G = GAF_GGX2(L, V, N, H, smooth, 128); //미세면 기하 함수.(Smith-GGX)★
    
    float n_l = dot(N, L);
    float n_v = dot(N, V);
    //n_l = max(n_l, 0);
    //n_v = max(n_v, 0);
    
    //return G / (4 * n_l * n_v);
    
    //float n_l = max(dot(N, L), 0);                            //<테스트1> 값 제한
    //float n_v = max(dot(N, V), 0);    
    //float n_l = saturate(max(dot(N, L), 0));                  //<테스트2> 값 포화도
    //float n_v = saturate(max(dot(N, V), 0));    
    //float n_l = saturate(max(dot(N, L), 0.001));              //<테스트3> 값 포화도 (0 방지)
    //float n_v = saturate(max(dot(N, V), 0.001));    
    
    //float4 specBRDF = (D * F * G)/(2 * n_l * n_v);            //2배 배율 (더 넓은 확산)
    float4 specBRDF = (D * F * G) / (4 * n_l * n_v); //Cook-Torrance 표준.
    //float4 specBRDF = (D * F * G)/4;                          //Cook-Torrance 표준 2 (GAF CT2 대응) ★
    //float4 specBRDF = (D * F * G)/(8 * PI * n_l * n_v);       //2π 감쇄 배울 추가.
    //float4 specBRDF = (D * F * G)/(16 * PI * n_l * n_v);      //4π 감쇄 배울 추가. (더 선명한 반사)

    
    //---------------------
    // 출력색 혼합 테스트.
    //---------------------
    //float4 spec = saturate(specBRDF);                         //Specular BRDF 출력.
    float4 spec = saturate(specBRDF) * 30; //Spec + 강도 조절. <주> g_Lit.Specular 미사용. "빛의 강도" 만 사용. 
    
    //이하 개별 테스트 (품질 확인)
    //float4 spec = F;
    //float4 spec = F * base;
    //float4 spec = specBRDF;
    //float4 spec = g_Lit.Specular * specBRDF;
    //float4 spec = lt * specBRDF;
    //float4 spec = lt * saturate(specBRDF);                     //기본멥+조명+Specular 혼합 
    //float4 spec = lt * saturate(specBRDF) * smooth;
    //float4 spec = lt * smooth + saturate(specBRDF) * smooth;    
    
    
    return spec;
}


void CalculateLight(float4 posv, float4 nrm, matrix mView, out float4 OutDiffuse, out float4 OutSpecular, out float4 OutEmissive)
{
    OutDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    OutSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    OutEmissive = (Material.Emissive * Material.BaseColor);
    
    for (uint i = 0; i < min(LightCnt, (uint) Light_MAX); ++i)
    {
        switch (Light[i].LightType)
        {
            case LightType_Direction:
                DirLight(posv, nrm, mView, Light[i], OutDiffuse, OutSpecular);
                break;
            
            case LightType_Point:
                PointLight(posv, nrm, mView, Light[i], OutDiffuse, OutSpecular);
                break;
            
            case LightType_Spot:
                //Col += SpotLight(posv, nrm, mView, Light[i], OutDiffuse, OutSpecular);
                break;
            
            case LightType_Rect:
                //Col += RectLight(posv, nrm, mView, Light[i], OutDiffuse, OutSpecular);
                break;
        }
    }
}


void DirLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular)
{
    float4 Diff = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Amb = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 Normal = float4(nrm.xyz, 0.0f);
    float4 N = Normal;
    N.w = 0; //4성분 되면서 w가 0이 아니게 될 수 있음.
    N = normalize(N);
    
    float4 L = float4(LightData.Direction.xyz, 0.0f);
    L = mul(mView, L);
    L.w = 0.0f;
    L = normalize(L);
    
    posv.w = 0;
    float4 V = normalize(-(posv));
    
    
    Diff = LambertLight(N, L, LightData.Diffuse, Material.Diffuse) * Material.BaseColor;
    Spec = SpecularBlinPhongLight(N, L, V, LightData.Specular, Material.Specular, Material.Roughness);
    Amb = LightData.Ambient * Material.Ambient * Material.BaseColor;
    
    OutDiffuse = OutDiffuse + (Diff + Amb) * LightData.LightColor;
    OutSpecular = OutSpecular + Spec * LightData.LightColor;

}

void PointLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular)
{
    float4 Diff = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Amb = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 ViewLightPosition = mul(mView, LightData.LightPosition);
    float4 L = float4((ViewLightPosition.xyz - posv.xyz), 0.0f);
    
    float4 Normal = float4(nrm.xyz, 0.0f);
    float4 N = Normal;
    N.w = 0;
    N = normalize(N);
    
    posv.w = 0;
    float4 V = normalize(-(posv));
    
    float Distance = max(length(L), 0.01);
    
    //광원 도달 거리 벗어남
    if (Distance > LightData.Range)
        return;
    
    //정규화
    L /= Distance;
    
    //거리 감쇠 공식
    float Under_att = (LightData.Attenuation.x + LightData.Attenuation.y * Distance + LightData.Attenuation.z * Distance * Distance);
    
    float att = 1 / max(Under_att, 0.0001f);
    
    //Ambient SmoothStep
    float Fade = 1.0f - smoothstep(LightData.Range * 0.8f, LightData.Range, Distance);

    
    Diff = LambertLight(N, L, LightData.Diffuse, Material.Diffuse) * att * Fade * Material.BaseColor;
    Spec = SpecularBlinPhongLight(N, L, V, LightData.Specular, Material.Specular, Material.Roughness) * att * Fade;
    Amb = LightData.Ambient * Material.Ambient * att * Fade * Material.BaseColor;
    
    OutDiffuse = OutDiffuse + (Diff + Amb) * LightData.LightColor;
    OutSpecular = OutSpecular + Spec * LightData.LightColor;
}

void SpotLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular)
{
    //미구현이므로 Light 연산을 하지 않음
    float4 Col = { 0.0f, 0.0f, 0.0f, 0.0f };
    OutDiffuse = OutDiffuse + Col;
    OutSpecular = OutSpecular + Col;
}

void RectLight(float4 posv, float4 nrm, matrix mView, LIGHT LightData, inout float4 OutDiffuse, inout float4 OutSpecular)
{
    //미구현이므로 Light 연산을 하지 않음
    float4 Col = { 0.0f, 0.0f, 0.0f, 0.0f };
    OutDiffuse = OutDiffuse + Col;
    OutSpecular = OutSpecular + Col;
}
#endif