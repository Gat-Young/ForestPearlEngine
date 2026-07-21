#include "FPMath.h"

float DegToRad(float degree)
{
	return degree * PI / 180.0f;
}

float RadToDeg(float rad)
{
	return rad * 180.0f / PI;
}

float Clamp(float value, float minValue, float maxValue)
{
	if (value < minValue) return minValue;
	if (value > maxValue) return maxValue;
	return value;
}

///////////////////////////////////////////////////////////////////////
//
// FPVector3 수학 함수
//

float FPVector3::LengthSq() const
{
	return x * x + y * y + z * z;
}

float FPVector3::Length() const
{
	return std::sqrt(this->LengthSq());
}


///////////////////////////////////////////////////////////////////////
//
// FPQuaternion 수학 함수
//
float FPQuaternion::LengthSq() const
{
	return x * x +
		y * y +
		z * z +
		w * w;
}

float FPQuaternion::Length() const
{
	return std::sqrt(this->LengthSq());
}



FPQuaternion FPQuaternion::Normalize() const
{
	float len = this->Length();

	if (len <= 0.000001f) return FPQuaternion{ 0, 0, 0, 1 };
	return FPQuaternion{ x / len , y/len, z/len, w/len };
}


FPVector3 Rotate(const FPQuaternion& quat, const FPVector3& vec)
{
	FPQuaternion Quatvec = { vec.x, vec.y, vec.z, 0.0f };

	FPQuaternion result = ((quat * Quatvec) * Inverse(quat));

	return FPVector3{result.x, result.y, result.z};
}

FPQuaternion Conjugate(const FPQuaternion& quat)
{
	return FPQuaternion{ -quat.x, -quat.y, -quat.z, quat.w };
}

FPQuaternion Inverse(const FPQuaternion& quat)
{
	float lenSq = quat.LengthSq();
	FPQuaternion ConQuat = Conjugate(quat);

	if (lenSq <= 0.000001f)
	{
		return FPQuaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
	}

	return FPQuaternion{ ConQuat.x / lenSq,
						 ConQuat.y / lenSq,
						 ConQuat.z / lenSq,
						 ConQuat.w / lenSq};
}

FPQuaternion AngleAxis(float angleRad, const FPVector3& axis)
{
	float half = angleRad * 0.5f;
	float s = std::sin(half);
	float c = std::cos(half);

	return FPQuaternion{axis.x * s, axis.y * s, axis.z * s, c}.Normalize();
}

FPVector3 FPQuaternion::ToEuler() const
{
	FPQuaternion q = Normalize();

	FPVector3 euler;

	// x = Pitch
	// y = Yaw
	// z = Roll

	float sinPitch = 2.0f * (q.w * q.x - q.y * q.z);
	sinPitch = Clamp(sinPitch, -1.0f, 1.0f);

	euler.x = std::asin(sinPitch);

	euler.y = std::atan2(
		2.0f * (q.w * q.y + q.x * q.z),
		1.0f - 2.0f * (q.x * q.x + q.y * q.y)
	);

	euler.z = std::atan2(
		2.0f * (q.w * q.z + q.x * q.y),
		1.0f - 2.0f * (q.x * q.x + q.z * q.z)
	);

	euler.x = RadToDeg(euler.x);
	euler.y = RadToDeg(euler.y);
	euler.z = RadToDeg(euler.z);

	return euler;
}


FPQuaternion FromEuler(const FPVector3& eulerDeg)
{
	float pitch = DegToRad(eulerDeg.x);
	float yaw = DegToRad(eulerDeg.y);
	float roll = DegToRad(eulerDeg.z);

	FPQuaternion qx = AngleAxis(pitch, FPVector3{ 1, 0, 0 });
	FPQuaternion qy = AngleAxis(yaw, FPVector3{ 0, 1, 0 });
	FPQuaternion qz = AngleAxis(roll, FPVector3{ 0, 0, 1 });

	FPQuaternion q = qy * qx * qz;	//Yaw-Pitch-Roll 순 <- 회전이 이동하다면 곱하는 순서를 고려할 것

	return q.Normalize();
}