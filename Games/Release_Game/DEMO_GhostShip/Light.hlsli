//조명 정보 구조체
struct LIGHT
{
    float4 Direction; //빛의 방향.
    float4 Diffuse; //주 광량 : 확산광 Diffuse Light.
    float4 Specular; //정반사 광량 : Specular Light.
    float4 Ambient; //보조 광량 : 주변광 Ambient Light.
    float Range; //빛 도달 거리.
    float Strength; //빛 강도.    
};

//재질 정보 구조체
struct MATERIAL
{
    float4 Diffuse; //확산광 반사율(%) 
    float4 Specular; //정반사광 반사율(%)
    float4 Ambient; //주변광 반사율(%) 
    float Power; //정반사 보정값 : "거칠기"
};


//조명 정보용 상수버퍼
cbuffer cbLIGHT : register(b1)
{
    LIGHT g_Lit;
};

//재질 정보용 상수버퍼
cbuffer cbMATERIAL : register(b2)
{
    MATERIAL g_Mtrl;
};


#define PI 3.141592f


// 조명 처리 함수.
//float4 DiffLight  (float4 nrm);
//float4 SpecLight  (float4 posv, float4 nrm);
float4 DiffLight(float4 nrm, matrix mWV, matrix mView);
float4 SpecLight(float4 posv, float4 nrm, matrix mWV, matrix mView);



/*
////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

VSOutput VS_Main(
				  float4 pos : POSITION,    //[입력] 정점좌표. Vertex Position (Model Space, 3D)
				  float4 nrm : NORMAL,      //[입력] 노멀 normal 
                  float2 uv  : TEXCOORD0    //[입력] 텍스처 좌표 Texture Coordiates.
				)
{
	//정점 정리
    //pos.w = 1;

    
    //----------------
    // 정점 변환.
    //----------------
    //...
    
    //----------------
    // 조명 계산.
    //---------------- 
    //조명 계산 1 (Diffuse Lighting)
    float4 diff = { 0, 0, 0, 1 };   //기본값 처리.
    if(g_Lit.bDiffOn) diff = DiffLight(nrm);

    //조명 계산 2 (Specular Lighting) 
    float4 spec = 0;
    if (g_Lit.bSpecOn) spec = SpecLight(posv, nrm);

    //조명을 모두 사용하지 않으면 기본값 처리.
    //if(!g_Lit.bDiffOn && !g_Lit.bSpecOn) diff = 1;

    //조명 없이 텍스처를 사용중이면 기본값 처리.(데모 시연용)
    if(!g_Lit.bDiffOn && g_Mtrl.bTexOn) diff = 1;
    
    
    //----------------
    // 정보 출력.
    //----------------
    VSOutput o = (VSOutput) 0;
    o.pos  = posh; 
    o.diff = diff;
    o.spec = spec;  
    o.uv   = uv;

    return o;
}
*/



////////////////////////////////////////////////////////////////////////////// 
//
//! 조명 계산 : 램버트 라이팅 모델 적용. Lambert Lighting Model
//!          : 뷰 공간 View Space 기준 처리.
//
float4 DiffLight(float4 nrm, matrix mWV, matrix mView)
{
    float4 N = nrm;
    N.w = 0;
    float4 L = g_Lit.Direction;
    L.w = 0;
    //float4 L = float4(g_Lit.Direction, 0);

    //뷰공간으로 정보를 변환.
    N = normalize(mul(N, mWV));
    L = normalize(mul(L, mView));

    //각 벡터 노멀라이즈.
    //N = normalize(N);
    //L = normalize(L);


    //조명 계산 
    float4 diff = max(dot(N, L), 0) * g_Lit.Diffuse * g_Mtrl.Diffuse;
    float4 amb = g_Lit.Ambient * g_Mtrl.Ambient;


	//포화도 처리 : 광량의 합을 1로 제한합니다.
	//return saturate(diff + amb);
    
    return diff + amb;
}



////////////////////////////////////////////////////////////////////////////// 
//
//! 정반사광 조명 계산 : 블린퐁 모델 적용. Blinn-Phong Lighting Model
//!                    : 뷰 공간 View Space 기준 처리.
//
float4 SpecLight(float4 posv, float4 nrm, matrix mWV, matrix mView)
{
    //데이터 정리.
    nrm.w = 0;
    posv.w = 0;
    
    float4 N = nrm;
    N.w = 0;
    float4 L = g_Lit.Direction;
    L.w = 0;

    //뷰공간으로 정보를 변환.
    N = normalize(mul(N, mWV));
    L = normalize(mul(L, mView));

    //시선백터 계산.
    float4 E = normalize(-posv);

    //하프벡터 계산.
    float4 H = normalize(L + E);

    //조명 계산 
    float4 spec = pow(max(dot(N, H), 0), g_Mtrl.Power) * g_Lit.Specular * g_Mtrl.Specular;
    spec.w = 1;
    
    return spec;
}




////////////////////////////////////////////////////////////////////////////// 
//
// Diffuse BRDF : 확산광 양방향 반사 분포 함수 (v0,v1)
// * 태양광, 1광속, 등방 조건.
// * 뷰공간 기준 처리
//
float4 Diffuse_BRDF_v1(float4 nrm, matrix mWV, matrix mView, float4 base, float4 ao)
{
    float4 N = nrm;
    N.w = 0;
    float4 L = g_Lit.Direction;
    L.w = 0;

    //뷰공간으로 정보를 변환.
    N = normalize(mul(N, mWV));
    L = normalize(mul(L, mView));
    
    //반사율 처리.
    float4 albedo = base * ao;
    
    //빛 강도 조절
    float4 lt = g_Lit.Diffuse * g_Lit.Strength;
    
    
    //조명 계산 : 태양광, 1광속, 등방 조건, diff = irradiance * (L.N) / π * albedo
    float4 diff = lt * max(dot(N, L), 0) / PI * albedo;
    
    //지역 간접광(Local illuminance) : IBL 적용 전까지는 간단하게 처리 합니다.
    float4 amb = g_Lit.Ambient * g_Lit.Strength * albedo; //주변광 강도 적용
    //float4 amb = g_Lit.Ambient * ao;                          //주변광 강도 무시 (비추)
    //float4 amb = g_Lit.Ambient / PI * albedo;                 //주변광 조사량(irradiance) 적용    
        
	//포화도 처리 : 광량의 합을 1로 제한한다.
	//return saturate(diff + amb);
    return diff + amb;
    
}




////////////////////////////////////////////////////////////////////////////// 
//
// Diffuse BRDF : 확산광 양방향 반사 분포 함수 (v2)
// * 태양광, 1광속, 등방 조건.
// * 뷰공간 기준 처리
//
float4 Diffuse_BRDF(float4 nrm, matrix mWV, matrix mView, float4 base, float4 metal, float4 ao)
{
    float4 N = nrm;
    N.w = 0;
    float4 L = g_Lit.Direction;
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
    float4 lt = g_Lit.Diffuse * g_Lit.Strength;
    //float4 lt =  diff0 * g_Lit.Strength;
    
    
    //조명 계산 : 태양광, 1광속, 등방 조건, diff = irradiance * (L.N) / π * albedo
    float4 diff = lt * max(dot(N, L), 0) / PI * albedo;
    
    //지역 간접광(Local illuminance) : IBL 적용 전까지는 간단하게 처리 합니다.
    float4 amb = g_Lit.Ambient * g_Lit.Strength * albedo; //주변광 강도 적용
    //float4 amb = g_Lit.Ambient * ao;                          //주변광 강도 무시 (비추)
    //float4 amb = g_Lit.Ambient / PI * albedo;                 //주변광 조사량(irradiance) 적용    
        
	//포화도 처리 : 광량의 합을 1로 제한한다.
	//return saturate(diff + amb);
    return diff + amb;
    //return diff;
    
}




////////////////////////////////////////////////////////////////////////////// 
//
// Frenel 반사 함수(F) (테스트용  v1)
// Schlick 근사 적용 : F(l,h) = R0 + (1-R0)(1-cos@)^5  
//
float4 SchlickFresnel_v1(float4 R0, float4 H, float4 L)
{
    float f0 = 1.0f - saturate(max(dot(H, L), 0));
    //float exp = pow(f0, 5);
    float4 Rf = R0 + (1 - R0) * pow(f0, 5); // Rf = R0 + (1-R0)(1-cos@)^5 
     
    return Rf;
}




////////////////////////////////////////////////////////////////////////////// 
//
//! Frenel 반사 함수(F) 
//! Schlick 근사 적용 : F(v,h) = R0 + (1-R0)(1-cos@)^5  
//! \param  V  시선백터
//! \param  H  하프벡터
//! \return 반사율
//
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





////////////////////////////////////////////////////////////////////////////// 
//
//! Frenel 반사 함수(F) 
//! Schlick 근사 적용 : F(v,h) = R0 + (1-R0)(1-cos@)^5  
//! R0 을 Base + Metalic 참조 혼합 (이쁨!)
//! \param  V  시선백터
//! \param  H  하프벡터
//! \return 반사율
//
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




////////////////////////////////////////////////////////////////////////////// 
//
//! NDF 분포 함수(D) 
//! BlinnPhong 모델 적용 : D(N,H) = ((α - 2)/2π)(cosθ)^α
//!
//! \param  N  노멀 백터
//! \param  H  하프 벡터
//! \param  smooth  미세면 노멀 편차 분포도, 입자별, 이미지 기반 처리 ( 1-rough map 사용)
//! \param  ka      미세면 노멀 편차 분포 계수 (1 ~ 128)
//! \return 반사율
//
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




////////////////////////////////////////////////////////////////////////////// 
//
//! 미세면 기하감쇠 함수 (G) (v0)
//! BlinnPhong NDF 대응 모델 없음 -> Cook-Torrance 대응 GAF 사용.
//! 참조 : https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
//!
//! \param  L  광원 벡터
//! \param  V  시야 벡터
//! \param  N  노멀 백터
//! \param  H  하프 벡터
//! \param  α  표준 편차
//! \return 반사율
//
float4 GAF_v0(float4 L, float4 V, float4 N, float4 H)
{
    //float a = smooth.r;                                               //미세면 거칠기, smooth  조절
    //float a = smooth.r * 128;                                         //미세면 거칠기, 광택, smooth 로 조절.(rough 값으로 반사 저하됨, 이전 데모와 유사한 광량확보를 위해 2배 보정 : 128)
    //float a = 1-smooth.r;                                             //미세면 거칠기, 보통 거침, 광택, rough  조절.
    
    //float4 Gm = (2 * dot(N, H) * dot(N, V)) / dot(V, H);
    //float4 Gs = (2 * dot(N, H) * dot(N, L)) / dot(V, H);
    //float4 G = min(1, min(Gm, Gs)); 
    
    float4 G = dot(N, L) * dot(N, V); //단축 방식 (Implicit) ★
    //float4 G = dot(N, L) * dot(N, V) * a;                             //단축 방식2 
    //float4 G = dot(N, L) * dot(N, V) * a / dot(V, H);                 //단축 방식3
    //float4 G = 2 * dot(N, L) * dot(N, V) / dot(V,H) ;                 //단축 방식4
    //float4 G = (dot(N, L) * dot(N, V)) / max(dot(N,L), dot(N, V));    //Newumman 방식
    //float4 G = (dot(N, L) * dot(N, V)) / pow(dot(V, H), 2);           //Kelemen 방식.
    return G;
}


////////////////////////////////////////////////////////////////////////////// 
//
//! 미세면 기하감쇠 함수 (G) (v1)
//! BlinnPhong NDF 대응 모델 없음 -> Cook-Torrance 대응 GAF 사용.
//! 참조 : https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
//!
//! \param  L  광원 벡터
//! \param  V  시야 벡터
//! \param  N  노멀 백터
//! \param  H  하프 벡터
//! \return 반사율
//
float4 GAF(float4 L, float4 V, float4 N, float4 H)
{
    //float a = 1;                                                      //미세면 거칠기, 고정(1)    
    
    float4 Gm = (2 * dot(N, H) * dot(N, V)) / dot(V, H);
    float4 Gs = (2 * dot(N, H) * dot(N, L)) / dot(V, H);
    float4 G = min(1, min(Gm, Gs)); //미세면 최소 거칠기, 고정 (1)
    return G;
}


float4 GAF_CT(float4 L, float4 V, float4 N, float4 H)
{
    //float a = 1;                                                      //미세면 거칠기, 고정(1)    
    
    float n_h = dot(N, H);
    float v_h = dot(V, H);
    float n_v = dot(N, V);
    float n_l = dot(N, L);
    
    //n_h = max(n_h, 0);
    //v_h = max(v_h, 0);
    //n_v = max(n_v, 0);
    //n_l = max(n_l, 0);     
    
    float4 Gm = (2 * n_h * n_v) / v_h;
    float4 Gs = (2 * n_h * n_l) / v_h;
    float4 G = min(1, min(Gm, Gs)); //미세면 최소 거칠기, 고정 (1)
    return G;
}



////////////////////////////////////////////////////////////////////////////// 
//
//! 미세면 기하감쇠 함수 (G) : Cook-Torrance 모델 (v2), 단축 버전 
//! BlinnPhong NDF 대응 모델 없음 
//!   --> Cook-Torrance GAF 모델 사용
//!   --> Cook-Torrance GAF 모델 (v2) 
//!        -->  G.ct(l, v, h) = min(1, (2(n.h)(n.v)/(v.h),  (2(n.h)(n.l))/(v.h))
//!        --   G.ct(l, v, h) / (n.l)(n.v) ≒ 1/(l.h)^2          //SIGGRAPH 2013 자료 대응.
//!
//! 참조 : https://blog.selfshadow.com/publications/s2013-shading-course/hoffman/s2013_pbs_physics_math_slides.pdf
//!
//
float4 GAF_CT2(float4 L, float4 V, float4 N, float4 H)
{
    float G = 1 / pow(dot(L, H), 2);
    
    return G;
}




////////////////////////////////////////////////////////////////////////////// 
//
//! 미세면 기하감쇠 함수 (G) (v2) : smooth 적용.
//! BlinnPhong NDF 대응 모델 없음 -> Cook-Torrance 대응 GAF 사용.
//! 참조 : https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
//!
//! \param  L  광원 벡터
//! \param  V  시야 벡터
//! \param  N  노멀 백터
//! \param  H  하프 벡터
//! \param  smooth  미세면 노멀 편차 분포도, 입자별, 이미지 기반 처리 ( 1-rough map 사용)
//! \param  ka      미세면 노멀 편차 분포 계수 (1 ~ 128)
//! \return 반사율
//
float4 GAF(float4 L, float4 V, float4 N, float4 H, float4 smooth, float4 ka)
{
    //float a = 1;                                                      //미세면 거칠기=0.
    //float a = smooth.r;                                               //미세면 거칠기, rough map 처리.
    //float a = smooth.r * 128;                                         //미세면 거칠기, 보통, 광택, smooth 로 조절.(D 함수와 동일 비례 처리)
    //float a = (1-smooth.r) * 128;                                     //미세면 거칠기, 보통, 광택, rough 로 조절.(D 함수와 동일 비례 처리)
    //a = a * a * a * a;                                                //미세면 거칠기 보정
    float a = smooth.r * ka; //ND 분표도, 광택량, rough 값으로 조절.    
    
    float Gm = (2 * dot(N, H) * dot(N, V)) / dot(V, H);
    float Gs = (2 * dot(N, H) * dot(N, L)) / dot(V, H);
    float G = min(a, min(Gm, Gs));
    //float G = min(Gm, Gs); 
    
    return G;
}




////////////////////////////////////////////////////////////////////////////// 
//
//! 미세면 기하감쇠 함수 (G) (v3) : Smith-Beckmann 모델 ★
//! BlinnPhong NDF 대응 모델 없음 -> Smith-Beckmann 모델 대응 [Walter, 2007]
//! 참조 : https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
//!
//! \param  L  광원 벡터
//! \param  V  시야 벡터
//! \param  N  노멀 백터
//! \param  H  하프 벡터
//! \param  smooth  미세면 노멀 편차 분포도, 입자별, 이미지 기반 처리 ( 1-rough map 사용)
//! \param  ka      미세면 노멀 편차 분포 계수 (1 ~ 128)
//! \return 반사율
//
float4 G1_Beckmann(float4 N, float4 V, float4 ka)
{
    float n_v = dot(N, V);
    //float n_v = max(dot(N, V), 0);

    //float g = dot(N, V) / (ka * sqrt(1-pow(dot(N, V), 2)));
    float c = n_v / (ka * sqrt(1 - pow(n_v, 2)));
    
    float g = 1;
    //if(c > 1.6) g = 1;
    if (c < 1.6)
        g = (3.535 * c + 2.181 * (c * c)) / (1 + 2.276 * c + 2.577 * (c * c));

    return g;
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


//! GAF : Smith-Beckman  
//! 
float4 GAF_SB(float4 L, float4 V, float4 N, float4 H, float4 smooth, float4 ka)
{
    //float a = 1;                                                      //미세면 거칠기=0.
    //float a = smooth.r;                                               //미세면 거칠기, rough map 처리.
    //float a = smooth.r * 128;                                         //미세면 거칠기, 보통, 광택, smooth 로 조절.(D 함수와 동일 비례 처리)
    //float a = (1-smooth.r) * 128;                                     //미세면 거칠기, 보통, 광택, rough 로 조절.(D 함수와 동일 비례 처리)
    //a = a * a * a * a;                                                //미세면 거칠기 보정
    //float a = smooth.r * ka;                                            //ND 분표도, 광택량, rough 값으로 조절.  (NDF.BlinnPhong 대응)   
    float a = 1 - smooth;
    
    float G1L = G1_Beckmann(N, L, a);
    float G1V = G1_Beckmann(N, V, a);
    float G = G1L * G1V;
    
    return G;
}



//! GAF : Schlick's Approximation of Smith-Beckmann
//! 
float4 GAF_SSB(float4 L, float4 V, float4 N, float4 H, float4 smooth, float4 ka)
{
    //float a = 1;                                                      //미세면 거칠기=0.
    //float a = smooth.r;                                               //미세면 거칠기, rough map 처리.
    //float a = smooth.r * 128;                                         //미세면 거칠기, 보통, 광택, smooth 로 조절.(D 함수와 동일 비례 처리)
    //float a = (1-smooth.r) * 128;                                     //미세면 거칠기, 보통, 광택, rough 로 조절.(D 함수와 동일 비례 처리)
    //a = a * a * a * a;                                                //미세면 거칠기 보정
    //float a = smooth.r * ka;                                            //ND 분표도, 광택량, rough 값으로 조절.  (NDF.BlinnPhong 대응)   
    float a = 1 - smooth;
    float k = a * sqrt(2 / PI);
    
    float G1L = G1_SB(N, L, k);
    float G1V = G1_SB(N, V, k);
    float G = G1L * G1V;
    
    return G;
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






////////////////////////////////////////////////////////////////////////////// 
//
// Specular Pre-BRDF : 레거시 BlinnPhong 개선 (v0, v1)
// * 태양광, 1광속, 등방 조건.
// * 뷰공간 기준 처리
//
float4 Specular_PreBRDF_v1(float4 posv, float4 nrm, matrix mWV, matrix mView, float4 texRough)
{
    float4 P = posv;
    P.w = 0;
    float4 N = nrm;
    N.w = 0;
    float4 L = g_Lit.Direction;
    L.w = 0;

    //뷰공간으로 정보를 변환.
    N = normalize(mul(N, mWV));
    L = normalize(mul(L, mView));

    //시선백터 계산.
    float4 E = normalize(-P);

    //하프벡터 계산.
    float4 H = normalize(L + E);

    //Rough map 사용 : 입자별 거칠기
    float4 smooth = saturate(1 - texRough);
    
    //빛 강도 조절
    float4 lt = g_Lit.Specular * g_Lit.Strength;
    
    //조명 계산 <1> BlinnPhong
    //float4 spec = pow(max(dot(N, H), 0), g_Mtrl.Power) * g_Lit.Specular * g_Mtrl.Specular;
    //#define m  g_Mtrl.Power
    
    //조명 계산 <2> BlinnPhong + Rough
    //float4 spec = g_Lit.Specular * pow(max(dot(N, H), 0), g_Mtrl.Power) * smooth;
    
    //조명 계산 <3>  Spcular BRDF 선행연구 
    //재질 미사용, Rough/Smooth 적용, smooth 가 1 이면 100% 광택)
    //float4 spec = g_Lit.Specular * pow(max(dot(N, H), 0), g_Mtrl.Power);
    float4 spec = lt * pow(max(dot(N, H), 0), g_Mtrl.Power) / PI;
    if (smooth.a)
        spec *= smooth;
    
    
    return spec;
}




////////////////////////////////////////////////////////////////////////////// 
//
// Specular Pre-BRDF :  정반사광 양방향 반사 분포 함수 (v2)(v3)
// * 태양광, 1광속, 등방 조건.
// * 뷰공간 기준 처리
//
float4 Specular_PreBRDF_v3(float4 posv, float4 nrm, matrix mWV, matrix mView, float4 base, float4 metal, float4 rough)
{
    float4 P = posv;
    P.w = 0;
    float4 N = nrm;
    N.w = 0;
    float4 L = g_Lit.Direction;
    L.w = 0;

    //뷰공간으로 정보를 변환.
    N = normalize(mul(N, mWV));
    L = normalize(mul(L, mView));

    //시선백터 계산.
    float4 E = normalize(-P);

    //하프벡터 계산.
    float4 H = normalize(L + E);

    
    //반사광량 계산. (Metalic - 비 Metallic 지역의 반사광 (색상) 결정) 
    //float4 spec0 = g_Lit.Specular * metal;                        //<방식1> Diffuse 혼합시 1 초과 가능성, 과도한 반사, 에너지 보존법치 위배 가능성 있음. 비추.
    float4 spec0 = lerp(0.08 * g_Lit.Specular, base, metal); //<방식2> Metal - 비 Metal 지역 보간. 
    //float4 spec0 = lerp( 0.08 * 0.5f, base, metal.r);             //<방식3> UE 엔진 기본값 spec = 0.5f
    
    //빛 강도 조절
    //float4 lt =  g_Lit.Specular * g_Lit.Strength;
    float4 lt = spec0 * g_Lit.Strength;
    
    //반사율 처리 : 입자별 거칠기. 
    float4 smooth = saturate(1 - rough);
    

    /*//조명 계산 <3>  Spcular BRDF 선행연구 (v2)
    //재질 미사용, Rough/Smooth 적용, smooth 가 1 이면 100% 광택)
    float4 spec = lt * pow(max(dot(N, H), 0), g_Mtrl.Power);
    //float4 spec = lt * pow(max(dot(N, H), 0), g_Mtrl.Power) / PI;
    if(smooth.a) spec *= smooth;    
    return spec;
    */
    
    // 조명 계산 <4> Spcular BRDF 선행연구 (v3) 
    // 반사율(표면 거칠기 Roughness ) : rough.map 적용 
    // 재질 미사용, Rough/Smooth 적용, smooth 가 1 이면 100% 광택)
    float4 spec = lt * pow(max(dot(N, H), 0.0f), (rough * 127.0f) + 1.0f); //rough 적용 (1 ~ 128) 
    //float4 spec = lt * pow(max(dot(N, H), 0.0f), rough * 100.0f);         //rough 적용 (0 ~ 100) 
    //float4 spec = lt * pow(max(dot(N, H), 0.0f), smooth * 100.0f);        //smooth 적용  풀질비교
    //if(smooth.a) spec *= smooth;                                          //smooth 적용(2). 품질비교 요망.
    return spec;
        
        
    /*
    //조명 계산 <1> BlinnPhong (비교용)
    //float4 spec = pow(max(dot(N, H), 0), g_Mtrl.Power) * g_Lit.Specular * g_Mtrl.Specular;
    //#define m  g_Mtrl.Power
    float4 spec = g_Lit.Specular * pow(max(dot(N, H), 0), g_Mtrl.Power);
    return spec;
    */
}




////////////////////////////////////////////////////////////////////////////// 
//
// Specular BRDF : 정반사광 양방향 반사 분포 함수
//
// * Cook-Torrance BRDF 모델 적용
//   - Frenel 반사 적용 (Schlick's approx)
//   - NDF 적용 (BlinnPhong NDF)
//
// * 태양광, 1광속, 등방 조건.
// * 뷰공간 기준 처리
//
float4 Specular_BRDF(float4 posv, float4 nrm, matrix mWV, matrix mView, float4 base, float4 metal, float4 rough)
{
    float4 P = posv;
    P.w = 0;
    float4 N = nrm;
    N.w = 0;
    float4 L = g_Lit.Direction;
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
    float4 spec0 = lerp(0.08 * g_Lit.Specular, base, metal); //<방식2> Metal - 비 Metal 지역 보간. 
    //float4 spec0 = lerp( 0.08 * 0.5f, base, metal.r);             //UE 엔진 기본값 spec = 0.5f
    
    //빛 강도 조절
    //float4 lt = g_Lit.Specular * g_Lit.Strength;
    float4 lt = spec0 * g_Lit.Strength;
    
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
    float4 spec = saturate(specBRDF) * g_Lit.Strength; //Spec + 강도 조절. <주> g_Lit.Specular 미사용. "빛의 강도" 만 사용. 
    
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




/**************** end of file "Light.fx" ***********************/
