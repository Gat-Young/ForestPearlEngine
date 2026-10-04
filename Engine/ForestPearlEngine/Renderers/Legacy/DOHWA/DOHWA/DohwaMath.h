#pragma once
#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////
//
//'도'->'라디안' 으로 변경 메크로.
// pi : 180 = r : 1 
// r = pi / 180      즉  1 라디안 : 0.017444444444444444

#define DOHWA_PI   3.141592f
#define DOHWAToRadian(a)  (a/180.0f * DOHWA_PI)
#define DOHWAToAngle(r)   (r * 180.0f / DOHWA_PI)
#define DOHWAToDegree(r)  DOHWAToAngle(r)

////////////////////////////////////////////////////////////////////////////
//
// 벡터 데이터
// 
//  멤버데이터를 행벡터로 나열했지만,
//  벡터/행렬 연산시에는 열벡터로 처리됨
//
///////////////////////////////////////////////////////////////////////////

// 2성분 벡터 구조체
struct DOHWAVECTOR2
{
	float x, y;
};

//DOHWAVECTOR2 일반 구조체 + 생성자/연산자 오버로딩
class DOHWAXVECTOR2 : public DOHWAVECTOR2
{
public:
	//float x, y;

public:
	DOHWAXVECTOR2() { x = 0; y = 0; }
	DOHWAXVECTOR2(float _x, float _y) { x = _x;  y = _y; }
	DOHWAXVECTOR2(const DOHWAVECTOR2& v) { x = v.x;  y = v.y; }

	//연산자 오버로딩
	DOHWAXVECTOR2 operator + (DOHWAXVECTOR2 rhs);
	DOHWAXVECTOR2 operator - (DOHWAXVECTOR2 rhs);
	operator DOHWAVECTOR2* ();


	operator POINT() { return POINT{ (LONG)x, (LONG)y }; }
};

//2성분 외적
float DOHWAXVec2CCW(DOHWAXVECTOR2* v0, DOHWAXVECTOR2* v1);

//2성분 Dot
float DOHWAXVec2Dot(DOHWAXVECTOR2* v0, DOHWAXVECTOR2* v1);

//2성분 벡터 길이 구하기
float B3YXVec2Length(DOHWAXVECTOR2* v);			
float B3YXVec2Length(float x, float y);			

//3성분 벡터 구조체
struct DOHWAVECTOR3
{
	float x, y, z;
};

//DOHWAVECTOR3 일반 구조체 + 생성자/연산자 오버로딩
class DOHWAXVECTOR3 : public DOHWAVECTOR3
{
public:
	//float x, y, z;

public:
	DOHWAXVECTOR3() { x = 0; y = 0; z = 0; }
	DOHWAXVECTOR3(float _x, float _y, float _z) { x = _x;  y = _y;   z = _z; }
	DOHWAXVECTOR3(const DOHWAVECTOR3& v) { x = v.x;  y = v.y;  z = v.z; }
	DOHWAXVECTOR3(const class DOHWAXVECTOR4& v);

	//연산자 오버로딩
	operator DOHWAXVECTOR2();
};

//전역 연산자 오버로딩
DOHWAXVECTOR3  operator - (DOHWAXVECTOR3 rhs);

//4성분 벡터 구조체
struct DOHWAVECTOR4
{
	float x, y, z, w;
};

//DOHWAVECTOR4 일반 구조체 + 생성자/연산자 오버로딩
class DOHWAXVECTOR4 : public DOHWAVECTOR4
{
public:
	//float x, y, z, w;
public:
	DOHWAXVECTOR4() { x = 0; y = 0; z = 0; w = 0; }
	DOHWAXVECTOR4(float _x, float _y, float _z, float _w) { x = _x;  y = _y;   z = _z;  w = _w; }
	DOHWAXVECTOR4(const DOHWAVECTOR4& v) { x = v.x;  y = v.y;  z = v.z; w = v.w; }
	DOHWAXVECTOR4(const DOHWAVECTOR3& v) { x = v.x; y = v.y; z = v.z; w = 1; }

	//연산자 오버로딩
	DOHWAXVECTOR4 operator = (const DOHWAXVECTOR3& rhs);
	operator DOHWAXVECTOR2();
};

// 벡터-행렬 곱 함수. <Dohwa> Out = v * M; ★
DOHWAXVECTOR4* DOHWAXVec4Transform(DOHWAXVECTOR4* pOut, CONST DOHWAXVECTOR4* pV, CONST class DOHWAXMATRIX* pM);

////////////////////////////////////////////////////////////////////////////////
// 
// DOHWAMATRIX 
// 행렬 구조체
//
struct DOHWAMATRIX
{
	union
	{
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

};



////////////////////////////////////////////////////////////////////////////////
// 
// DOHWAXMATRIX 
// 행렬 클래스
// 
// DX 행렬 클래스를 참고하여 필요한 기능을 구현하십시오.
// [D3DXMATRIX](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrix)
// [XMMATRIX](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/ns-directxmath-xmmatrix)
//
class DOHWAXMATRIX : public DOHWAMATRIX
{
public:
	//4X4 행렬 기능 추가....(필요시)(과제) ★ 

public:
	DOHWAXMATRIX() {}
	DOHWAXMATRIX(const DOHWAMATRIX& m) { 
		_11 = m._11; _12 = m._12; _13 = m._13; _14 = m._14;
		_21 = m._21; _22 = m._22; _23 = m._23; _24 = m._24;
		_31 = m._31; _32 = m._32; _33 = m._33; _34 = m._34;
		_41 = m._41; _42 = m._42; _43 = m._43; _44 = m._44;
	}

	DOHWAXMATRIX(	float m11, float m12, float m13, float m14,
					float m21, float m22, float m23, float m24, 
					float m31, float m32, float m33, float m34, 
					float m41, float m42, float m43, float m44) 
	{
		_11 = m11; _12 = m12; _13 = m13; _14 = m14;
		_21 = m21; _22 = m22; _23 = m23; _24 = m24;
		_31 = m31; _32 = m32; _33 = m33; _34 = m34;
		_41 = m41; _42 = m42; _43 = m43; _44 = m44;
	}

	//연산자 오버로딩도 필요합니다.(필요시)(과제) ★

    DOHWAXMATRIX operator * (const DOHWAXMATRIX& rhs);

};
typedef DOHWAXMATRIX MATRIX;


//행렬 단위화(초기화) 함수 <DX> Out = I. 
//D3DXMATRIX* D3DXMatrixIdentity ( D3DXMATRIX *pOut );
//행렬 단위화(초기화) 함수 <DOHWA> Out = I. ★
DOHWAXMATRIX* DOHWAXMatrixIdentity(DOHWAXMATRIX* pOut);

#define Radian(degree) (degree * 3.141592f / 180.0f)

//모델링 행렬을 위한 수학 함수
void DOHWAXMatrixScale(DOHWAXMATRIX* Matrix, float Scale[3]);

//Pitch
void DOHWAXMatrixRotationPitch(DOHWAXMATRIX* Matrix, float Rotation);

//Yaw
void DOHWAXMatrixRotationYaw(DOHWAXMATRIX* Matrix, float Rotation);

//Roll
void DOHWAXMatrixRotationRoll(DOHWAXMATRIX* Matrix, float Rotation);

void DOHWAXMatrixRotation(DOHWAXMATRIX* Matrix, float Rotation[3]);

void DOHWAXMatrixTransform(DOHWAXMATRIX* Matrix, float Transform[3]);


//선형 보간
float DOHWAXLerp(float start, float end, float t);

////////////////////////////////////////////////////////////////////////////
//
// 컬러 구조체
// 
struct DOHWACOLOR4
{
	float r, g, b, a;
};

////////////////////////////////////////////////////////////////////////////////
//
// DOHWAXCOLOR
//
//
class DOHWAXCOLOR : public DOHWACOLOR4
{
public:
	//float r, g, b, a;

public:
	DOHWAXCOLOR() { r = g = b = 0; a = 1.0f; }	//기본색 (0,0,0,a=1) 
	DOHWAXCOLOR(float _r, float _g, float _b, float _a = 1) { r = _r; g = _g; b = _b; a = _a; }
	DOHWAXCOLOR(BYTE _r, BYTE _g, BYTE _b, BYTE _a);						
	DOHWAXCOLOR(DWORD col);		

	DOHWAXCOLOR operator* (DOHWAXCOLOR rhs);		//색상 채널별 곱.★
	DOHWAXCOLOR operator* (float rhs);				//색상 스칼라 곱.★
	DOHWAXCOLOR operator+ (DOHWAXCOLOR rhs);		//색상 덧셈 (채널별)★
	DOHWAXCOLOR operator- (DOHWAXCOLOR rhs);		//색상 뺄셈 (채널별)★
	operator DWORD ();								//DWORD 형변환.★
};
typedef DOHWAXCOLOR COLOR;

//선형보간 (Linear Interpolation) 함수 
void  DOHWAXLerp(int* v, int v0, int v1, float a);
void  DOHWAXLerp(DOHWAXCOLOR* c, DOHWAXCOLOR c0, DOHWAXCOLOR c1, float a);	//색상(DOHWAXCOLOR)
