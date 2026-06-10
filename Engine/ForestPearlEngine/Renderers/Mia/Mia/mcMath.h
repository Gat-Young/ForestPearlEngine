#pragma once

////////////////////////////////
// '도'->'라디안' 으로 변경 메크로
#define MC_PI	3.141592f
//#define mcConvertToRadian(deg)	((float)(YN_PI/180.0f)*((float)deg))
//#define mcConvertToDegree(rad)	((rad) * 180.0f / MC_PI)
#define mcToRadian(a)  (a/180.0f * YN_PI)
#define mcToAngle(r)   (r * 180.0f / YN_PI)
#define mcToDegree(r)  mcToAngle(r)


////////////////////////////////
// 벡터 데이터
// 2성분 벡터 구조체
struct B3MVECTOR2 {
	float x, y;
};


// 3성분 벡터 구조체
struct B3MVECTOR3 {
	float x, y, z;
};


// 4성분 벡터.구조체
struct B3MVECTOR4 {
	float x, y, z, w;
};


////////////////////////////////
// 벡터 클래스 (2성분)
class B3MXVECTOR2 : public B3MVECTOR2
{
public:
	B3MXVECTOR2() { x = 0;	 y = 0; }
	B3MXVECTOR2(float _x, float _y) { x = _x;	 y = _y; }
	B3MXVECTOR2(const B3MVECTOR2& v) { x = v.x;  y = v.y; }

	//연산자 오버로딩
	B3MXVECTOR2 operator + (B3MXVECTOR2 rhs);
	B3MXVECTOR2 operator - (B3MXVECTOR2 rhs);
	B3MVECTOR2	operator = (B3MXVECTOR2 rhs);
	operator B3MVECTOR2* ();

	operator POINT() { return POINT{ (LONG)x, (LONG)y }; }
};

typedef B3MXVECTOR2 VECTOR2;

float B3MXVec2CCW(B3MXVECTOR2* v0, B3MXVECTOR2* v1);


////////////////////////////////
// 벡터 클래스 (3성분)
class B3MXVECTOR3 : public B3MVECTOR3
{
public:
	//float x, y, z;
public:
	B3MXVECTOR3() { x = y = z = 0; }
	B3MXVECTOR3(float _x, float _y, float _z) { x = _x;  y = _y;   z = _z; }
	B3MXVECTOR3(const B3MVECTOR3& v) { x = v.x; y = v.y;  z = v.z; }
};

typedef B3MXVECTOR3 VECTOR3;


////////////////////////////////
// 벡터 클래스 (4성분)
class B3MXVECTOR4 : public B3MVECTOR4
{
public:
	//float x, y, z, w;
public:
	B3MXVECTOR4() { x = y = z = w = 0; }
	B3MXVECTOR4(float _x, float _y, float _z, float _w) { x = _x;  y = _y;   z = _z;  w = _w; }
	B3MXVECTOR4(const B3MVECTOR4& v) { x = v.x; y = v.y;  z = v.z; w = v.w; }

};
typedef B3MXVECTOR4 VECTOR4;


////////////////////////////////
// 컬러 구조체
struct B3MCOLOR
{
	float r, g, b, a;
};

////////////////////////////////
// 컬러 클래스
class B3MXCOLOR : public B3MCOLOR
{
public:
	B3MXCOLOR() { r = g = b = 0; a = 1.0f; }	//기본색 (0,0,0,a=1)
	B3MXCOLOR(float _r, float _g, float _b, float _a = 1) { r = _r; g = _g; b = _b; a = _a; }
	B3MXCOLOR(DWORD col);

	B3MXCOLOR operator* (B3MXCOLOR rhs);
	B3MXCOLOR operator* (float rhs);
	B3MXCOLOR operator+ (B3MXCOLOR rhs);
	B3MXCOLOR operator- (B3MXCOLOR rhs);
	operator DWORD ();
};

typedef B3MXCOLOR COLOR;
#define TOCOLOR(col)  (*(COLOR*)&(col))

////////////////////////////////
// 선형보간
void MCLerp(int* V, int V0, int V1, float A);
void MCLerp(DWORD* C, DWORD C0, DWORD C1, float A);