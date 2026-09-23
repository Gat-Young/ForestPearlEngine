#pragma once
#include <cmath>
#include <DirectXMath.h>
constexpr float PI = 3.14159265358979323846f;


//범용 수학 함수
float DegToRad(float degree);
float RadToDeg(float rad);

float Clamp(float value, float minValue, float maxValue);

struct FPVector2
{
	float x = 0;
	float y = 0;

	explicit operator float() const { return x; }
	explicit operator bool() const { return x != 0.f || y != 0.f; }
};

struct FPVector3
{
	float x = 0;
	float y = 0;
	float z = 0;

	FPVector3 operator -() const
	{
		FPVector3 ret;
		ret.x = -x;
		ret.y = -y;
		ret.z = -z;
		return ret;
	}

	FPVector3 operator -(const FPVector3& rhs) const
	{
		FPVector3 ret;
		ret.x = x -rhs.x;
		ret.y = y -rhs.y;
		ret.z = z -rhs.z;
		return ret;
	}

	FPVector3 operator +(const FPVector3& rhs) const
	{
		FPVector3 ret;
		ret.x = x + rhs.x;
		ret.y = y + rhs.y;
		ret.z = z + rhs.z;
		return ret;
	}

	FPVector3 operator *(const float& rhs) const
	{
		FPVector3 ret;
		ret.x = x * rhs;
		ret.y = y * rhs;
		ret.z = z * rhs;
		return ret;
	}

	FPVector3 operator *(const FPVector3& rhs) const
	{
		FPVector3 ret;
		ret.x = x * rhs.x;
		ret.y = y * rhs.y;
		ret.z = z * rhs.z;
		return ret;
	}

	FPVector3 operator =(const FPVector3& rhs) const
	{
		FPVector3 ret;
		ret.x = rhs.x;
		ret.y = rhs.y;
		ret.z = rhs.z;
		return ret;
	}

	float LengthSq() const;

	float Length() const;

	FPVector3 Normalize() const;

};

struct FPVector4
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;

	FPVector4 operator *(const float& rhs) const
	{
		FPVector4 ret;
		ret.x = x * rhs;
		ret.y = y * rhs;
		ret.z = z * rhs;
		ret.w = w * rhs;
		return ret;
	}

	FPVector4 operator =(const FPVector4& rhs) const
	{
		FPVector4 ret;
		ret.x = rhs.x;
		ret.y = rhs.y;
		ret.z = rhs.z;
		ret.w = rhs.w;
		return ret;
	}
};

struct FPQuaternion
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;

	FPQuaternion operator *(const FPQuaternion& rhs) const
	{
		FPQuaternion ret;
		ret.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		ret.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		ret.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
		ret.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;

		return ret;
	}

	operator FPVector4() const
	{
		FPVector4 out;
		out.x = x;
		out.y = y;
		out.z = z;
		out.w = w;

		return out;
	}

	FPQuaternion operator =(const FPQuaternion& rhs) const
	{
		FPQuaternion ret;
		ret.x = rhs.x;
		ret.y = rhs.y;
		ret.z = rhs.z;
		ret.w = rhs.w;
		return ret;
	}

	float LengthSq() const;

	float Length() const;

	FPVector3 ToEuler() const;

	FPQuaternion Normalize() const;

};

struct FPMatrix
{
	DirectX::XMMATRIX Matrix = DirectX::XMMatrixIdentity();

	FPMatrix() : Matrix(DirectX::XMMatrixIdentity()) {}
	
	FPMatrix(DirectX::XMMATRIX Matrix) : Matrix(Matrix) {}

	//Matrix 곱
	FPMatrix operator*(const FPMatrix& rhs) const
	{
		return FPMatrix(Matrix * rhs.Matrix);
	}

	//Scalar 곱
	FPMatrix operator*(const float& rhs) const
	{
		return FPMatrix(rhs * Matrix);
	}

	FPMatrix operator*(const int& rhs) const
	{
		return FPMatrix(rhs * Matrix);
	}

	//역 행렬
	FPMatrix MatrixInverse()
	{
		return FPMatrix(DirectX::XMMatrixInverse(nullptr, Matrix));
	}

};

FPVector3 Rotate(const FPQuaternion& quat, const FPVector3& vec);

FPQuaternion Conjugate(const FPQuaternion& quat);

FPQuaternion Inverse(const FPQuaternion& quat);

FPQuaternion FromEuler(const FPVector3& eulerDeg);

FPQuaternion AngleAxis(float angleRad, const FPVector3& axis);


//Transform 행렬
FPMatrix MatrtixTranslation(const float& x, const float& y, const float& z);

FPMatrix MatrtixTranslation(const FPVector3& Location);

//Rotation 행렬

//RollPitchYaw
FPMatrix MatrixRotaionRollPitch(const float& PitchDegree, const float& YawDegree, const float& RollDegree);

//Quternion
FPMatrix MatrixRotationQuaternion(const FPQuaternion& quat);

//Scale 행렬
FPMatrix MatrixScaling(const float& x, const float& y, const float& z);

FPMatrix MatrixScaling(const FPVector3& Scale);


//View 행렬

//왼손 좌표계 용
FPMatrix MatrixLookAtLH(const FPVector3& eye, const FPVector3& target, const FPVector3& up);

FPMatrix MatrixLookToLH(const FPVector3& eye, const FPVector3& direction, const FPVector3& up);

//Projection 행렬
FPMatrix MatrixPerspectiveFovLH(const float& Fov, const float& Aspect, const float& Zn, const float& Zf);
