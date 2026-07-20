#pragma once

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

	float LengthSq() const;

	float Length() const;

	FPVector3 ToEuler() const;

	FPQuaternion Normalize() const;

};

FPVector3 Rotate(const FPQuaternion& quat, const FPVector3& vec);

FPQuaternion Conjugate(const FPQuaternion& quat);

FPQuaternion Inverse(const FPQuaternion& quat);

FPQuaternion FromEuler(const FPVector3& eulerDeg);

FPQuaternion AngleAxis(float angleRad, const FPVector3& axis);


