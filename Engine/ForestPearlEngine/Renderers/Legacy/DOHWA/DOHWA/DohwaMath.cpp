#include "Windows.h"  
#include "stdio.h"
#include "stdlib.h" 
#include "math.h"

#include "DohwaMath.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// struct COLOR
//
///////////////////////////////////////////////////////////////////////////////

//
// 생성자 오버로딩.
//
// BYTE(r, g, b, a) -> float (r, g, b, a)
//
DOHWAXCOLOR::DOHWAXCOLOR(BYTE _r, BYTE _g, BYTE _b, BYTE _a)
{
	b = _b/255.0f;					//실수형으로 전환.  0~ 1.0f
	g = _g/255.0f;
	r = _r/255.0f;
	a = _a/255.0f;
}

// DWORD 형 색상에서 float 타입으로 색상정보를 전환합니다.★
// DWORD(a, r, g, b) --> float(r, g, b, a) 로 전환. (GDI용)  
//   0x AA RR GG BB 
DOHWAXCOLOR::DOHWAXCOLOR(DWORD col)
{
	// 필요 코드를 완성하십시요.
	//

	DWORD A = (col & 0xff000000) >> 24;
	DWORD R = (col & 0x00ff0000) >> 16;
	DWORD G = (col & 0x0000ff00) >> 8;
	DWORD B = (col & 0x000000ff);

	//<Blue>> 변환
	b = B / 255.0f;						//여기서 다시 실수형으로 전환.  0~ 1.0f 

	//<Green> 변환
	g = G / 255.0f;

	//<Red> 변환
	r = R / 255.0f;

	//<Alpha> 변환.
	a = A / 255.0f;

}




// 
// 색상 연산 오버로딩/
//
// 모든 연산에 색상 포화도(Saturation) 제한을 처리합니다.
// 0 ~ 1.0f ( 0~ 255)
//

// 색상 채널별 곱셈.  
DOHWAXCOLOR DOHWAXCOLOR::operator * (DOHWAXCOLOR rhs)
{
	DOHWAXCOLOR v;

	v.r = min(max((r * rhs.r), 0),  1);
	v.g = min(max((g * rhs.g), 0) , 1);
	v.b = min(max((b * rhs.b), 0) , 1);
	v.a = min(max((a * rhs.a), 0) , 1);
	

	return v;
}


// 색상 스칼라 곱.: 
DOHWAXCOLOR DOHWAXCOLOR::operator * (float rhs)
{
	DOHWAXCOLOR v;

	v.r = min(max((r * rhs), 0), 1);
	v.g = min(max((g * rhs), 0), 1);
	v.b = min(max((b * rhs), 0), 1);
	v.a = min(max((a * rhs), 0), 1);

	return v;
}

// 색상 혼합.★
DOHWAXCOLOR DOHWAXCOLOR::operator + (DOHWAXCOLOR rhs)
{
	DOHWAXCOLOR v;

	v.r = min(max(r + rhs.r, 0), 1);
	v.g = min(max(g + rhs.g, 0), 1);
	v.b = min(max(b + rhs.b, 0), 1);
	v.a = min(max(a + rhs.a, 0), 1);

	return v;
}

// 색상 뺄셈.★
DOHWAXCOLOR DOHWAXCOLOR::operator - (DOHWAXCOLOR rhs)
{
	DOHWAXCOLOR v;

	v.r = min(max(r - rhs.r, 0), 1);
	v.g = min(max(g - rhs.g, 0), 1);
	v.b = min(max(b - rhs.b, 0), 1);
	v.a = min(max(a - rhs.a, 0), 1);

	return v;
}


// GDI 대응 형변환
// float(r, g, b, a) -> DWORD(a, b, g, r) 로 전환. (GDI용) 
// Blue 채널이 16번비트..주의.★
//
DOHWAXCOLOR::operator DWORD ()
{
	COLORREF col;

	DWORD A = ((DWORD)(a * 255) & 0x000000ff) << 24;
	DWORD R = ((DWORD)(r * 255) & 0x000000ff);
	DWORD G = ((DWORD)(g * 255) & 0x000000ff) << 8;
	DWORD B = ((DWORD)(b * 255) & 0x000000ff) << 16;

	col = B | G | R;
	return col;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// VECTOR2 :  2성분 벡터 연산자 오버로딩
//
///////////////////////////////////////////////////////////////////////////////

DOHWAXVECTOR2 DOHWAXVECTOR2::operator + (DOHWAXVECTOR2 rhs)
{
	DOHWAVECTOR2 v;
	v.x = x + rhs.x;
	v.y = y + rhs.y;

	return v;
}


DOHWAXVECTOR2 DOHWAXVECTOR2::operator - (DOHWAXVECTOR2 rhs)
{
	DOHWAVECTOR2 v;
	v.x = x - rhs.x;
	v.y = y - rhs.y;

	return v;
}



DOHWAXVECTOR2::operator DOHWAVECTOR2* ()
{
	return (DOHWAVECTOR2*)this;
}


///////////////////////////////////////////////////////////////////////////////
//
// 2성분 외적 : 외적의 결과가 양수면, 'CCW' .Back-Face Culling 에 사용.
//			  : D3DXVec2CCW 대응
//  
// 2성분으로는 구조적으로 외적이 불가능하므로 3차원으로 확장 사용합니다. 
//		v0=(x1,y1,0) ⓧ v1=(x2,y2,0). 
//
// 또는 아래 수식을 참조
//
// [3성분 외적] 
// VECTOR3 v;
// v.x = v0.y * v1.z - v0.z * v1.y;
// v.y = v0.z * v1.x - v0.x * v1.z;
// v.z = v0.x * v1.y - v0.y * v1.x; <- 2차원에서 사용되는 값
//
float DOHWAXVec2CCW(DOHWAXVECTOR2* v0, DOHWAXVECTOR2* v1)
{
	float z = v0->x * v1->y - v0->y * v1->x;
	return z;
}

float DOHWAXVec2Dot(DOHWAXVECTOR2* v0, DOHWAXVECTOR2* v1)
{
	return (v0->x * v1->x + v0->y * v1->y);
}

//선형 보간
float DOHWAXLerp(float start, float end, float t)
{
	t = min(max(t, 0), 1);
	return start * (1 - t) + end * t;
}


///////////////////////////////////////////////////////////////////////////////
//
//  2성분 벡터 : 길이 구하기.
//
float DOHWAXVec2Length(DOHWAXVECTOR2* v)
{
	float len = sqrtf((v->x * v->x) + (v->y * v->y));

	return len;
}


float DOHWAXVec2Length(float x, float y)
{
	float len = sqrtf((x * x) + (y * y));

	return len;
}

///////////////////////////////////////////////////////////////////////////////
//
// class B3YXVECTOR3
//
///////////////////////////////////////////////////////////////////////////////

DOHWAXVECTOR3::DOHWAXVECTOR3(const DOHWAXVECTOR4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}



DOHWAXVECTOR3::operator DOHWAXVECTOR2 ()         
{
	DOHWAXVECTOR2 v(x, y);
	return v;
}

///////////////////////////////////////////////////////////////////////////////
//
// class B3YXVECTOR4  
//
///////////////////////////////////////////////////////////////////////////////

DOHWAXVECTOR4 DOHWAXVECTOR4::operator=(const DOHWAXVECTOR3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = 1.0f;

	return *this;
}


DOHWAXVECTOR4::operator DOHWAXVECTOR2 ()
{
	DOHWAXVECTOR2 v(x, y);
	return v;
}


////////////////////////////////////////////////////////////////////////////////
// 
//	DOHWAXLerp : 선형보간 : 정수형 
// 
//	v	보간된 값
//	v0	시작 값
//	v1	끝 값
//	a	보간율 (alpha : 0.0~1.0)

void DOHWAXLerp(int* v, int v0, int v1, float a)
{
	//if (a < 0) a = 0;	if (a > 1) a = 1;
	a = min(max(a, 0), 1);					//0~1 제한

	*v = (int)((float)v0 * (1 - a) + (float)v1 * (a));
}


////////////////////////////////////////////////////////////////////////////////
//
// DOHWAXLerp 색상 선형보간.
//
// 
//	c	보간된 색상
//	c0	시작 색
//	c1	끝 색
//	a	보간율 (alpha : 0.0~1.0)
//
void DOHWAXLerp(DOHWAXCOLOR* c, DOHWAXCOLOR c0, DOHWAXCOLOR c1, float a)  //★
{
	a = min(max(a, 0), 1);					//0~1 제한

	*c = c0 * (1 - a) + c1 * (a);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//	행렬
//
////////////////////////////////////////////////////////////////////////////////////////

//! \brief	벡터-행렬 곱 함수. 
//!			Out = v * M;
//! \todo2	[과제] 벡터-행렬 곱 함수 만들기
//!
DOHWAXVECTOR4* DOHWAXVec4Transform(DOHWAXVECTOR4* pOut, CONST DOHWAXVECTOR4* pV, CONST DOHWAXMATRIX* pM)
{
	DOHWAXVECTOR4 v;

	v.x = (pV->x * pM->_11) + (pV->y * pM->_21) + (pV->z * pM->_31) + (pV->w * pM->_41);
	v.y = (pV->x * pM->_12) + (pV->y * pM->_22) + (pV->z * pM->_32) + (pV->w * pM->_42);
	v.z = (pV->x * pM->_13) + (pV->y * pM->_23) + (pV->z * pM->_33) + (pV->w * pM->_43);
	v.w = (pV->x * pM->_14) + (pV->y * pM->_24) + (pV->z * pM->_34) + (pV->w * pM->_44);

	if (pOut) *pOut = v;		//결과를 외부로 복사..

	return pOut;
}

DOHWAXMATRIX* DOHWAXMatrixIdentity(DOHWAXMATRIX* pOut)
{
	DOHWAXMATRIX m;
	memset(m.m, 0, sizeof(DOHWAXMATRIX));

	m._11 = m._22 = m._33 = m._44 = 1;

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}

void DOHWAXMatrixScale(DOHWAXMATRIX* Matrix, float Scale[3])
{
	DOHWAXMATRIX ScaleMatrix= 
	{
			Scale[0], 0.0f, 0.0f, 0.0f,
			0.0f, Scale[1], 0.0f, 0.0f,
			0.0f, 0.0f, Scale[2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
	};

	*Matrix = (*Matrix) * ScaleMatrix;
}

//Pitch
void DOHWAXMatrixRotationPitch(DOHWAXMATRIX* Matrix, float Rotation)
{
	DOHWAXMATRIX RotationMatrixPitch =
	{
		1.0f, 0.0f,                   0.0f,                     0.0f,
		0.0f, cosf(Radian(Rotation)), sinf(Radian(Rotation)),  0.0f,
		0.0f, -sinf(Radian(Rotation)), cosf(Radian(Rotation)),   0.0f,
		0.0f, 0.0f,                   0.0f,                     1.0f,
	};

	(*Matrix) = (*Matrix) * RotationMatrixPitch;
}

//Yaw
void DOHWAXMatrixRotationYaw(DOHWAXMATRIX* Matrix, float Rotation)
{
	DOHWAXMATRIX RotationMatrixYaw =
	{
		cosf(Radian(Rotation)),     0.0f,                   -sinf(Radian(Rotation)),     0.0f,
		0.0f,                       1.0f,                   0.0f,                       0.0f,
		sinf(Radian(Rotation)),    0.0f,                   cosf(Radian(Rotation)),     0.0f,
		0.0f,                       0.0f,                   0.0f,                       1.0f,
	};

	(*Matrix) = (*Matrix) * RotationMatrixYaw;
}

//Roll
void DOHWAXMatrixRotationRoll(DOHWAXMATRIX* Matrix, float Rotation)
{
	DOHWAXMATRIX RotationMatrixRoll =
	{
		cosf(Radian(Rotation)),     sinf(Radian(Rotation)),                   0.0f,                     0.0f,
		-sinf(Radian(Rotation)),     cosf(Radian(Rotation)),                    0.0f,                     0.0f,
		0.0f,                       0.0f,                                      1.0f,                     0.0f,
		0.0f,                       0.0f,                                      0.0f,                     1.0f,
	};

	(*Matrix) = (*Matrix) * RotationMatrixRoll;
}
void DOHWAXMatrixRotation(DOHWAXMATRIX* Matrix, float Rotation[3])
{
	DOHWAXMATRIX RotationMatrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	DOHWAXMatrixRotationRoll(&RotationMatrix, Rotation[2]);
	DOHWAXMatrixRotationPitch(&RotationMatrix, Rotation[0]);
	DOHWAXMatrixRotationYaw(&RotationMatrix, Rotation[1]);

	(*Matrix) = (*Matrix) * RotationMatrix;
}

void DOHWAXMatrixTransform(DOHWAXMATRIX* Matrix, float Transform[3])
{
	DOHWAXMATRIX TransformMatrix =
	{
		1.0f,         0.0f,         0.0f,         0.0f,
		0.0f,         1.0f,         0.0f,         0.0f,
		0.0f,         0.0f,         1.0f,         0.0f,
		Transform[0], Transform[1], Transform[2], 1.0f,
	};

	(*Matrix) = (*Matrix) * TransformMatrix;

}

DOHWAXMATRIX DOHWAXMATRIX::operator*(const DOHWAXMATRIX& rhs)
{
	DOHWAXMATRIX MultiMatrix;

	MultiMatrix._11 = _11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31 + _14 * rhs._41;
	MultiMatrix._12 = _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32 + _14 * rhs._42;
	MultiMatrix._13 = _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33 + _14 * rhs._43;
	MultiMatrix._14 = _11 * rhs._14 + _12 * rhs._24 + _13 * rhs._34 + _14 * rhs._44;

	MultiMatrix._21 = _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31 + _24 * rhs._41;
	MultiMatrix._22 = _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32 + _24 * rhs._42;
	MultiMatrix._23 = _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33 + _24 * rhs._43;
	MultiMatrix._24 = _21 * rhs._14 + _22 * rhs._24 + _23 * rhs._34 + _24 * rhs._44;

	MultiMatrix._31 = _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31 + _34 * rhs._41;
	MultiMatrix._32 = _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32 + _34 * rhs._42;
	MultiMatrix._33 = _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33 + _34 * rhs._43;
	MultiMatrix._34 = _31 * rhs._14 + _32 * rhs._24 + _33 * rhs._34 + _34 * rhs._44;

	MultiMatrix._41 = _41 * rhs._11 + _42 * rhs._21 + _43 * rhs._31 + _44 * rhs._41;
	MultiMatrix._42 = _41 * rhs._12 + _42 * rhs._22 + _43 * rhs._32 + _44 * rhs._42;
	MultiMatrix._43 = _41 * rhs._13 + _42 * rhs._23 + _43 * rhs._33 + _44 * rhs._43;
	MultiMatrix._44 = _41 * rhs._14 + _42 * rhs._24 + _43 * rhs._34 + _44 * rhs._44;

	return MultiMatrix;
}
