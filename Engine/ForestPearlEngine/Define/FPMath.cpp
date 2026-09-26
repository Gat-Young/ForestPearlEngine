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

FPVector3 FPVector3::Normalize() const
{
	float len = this->Length();

	if (len <= 0.000001f) return FPVector3{ 0, 0, 0};
	return FPVector3{ x / len , y / len, z / len };
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

	FPQuaternion BaseQuat = quat.Normalize();

	FPQuaternion result = ((BaseQuat * Quatvec) * Inverse(BaseQuat));

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

float NormalizeAxis(float Angle)
{
	if (!std::isfinite(Angle))
	{
		return 0.0f;
	}

	Angle = std::fmod(Angle, 360.0f);

	if(Angle > 180.0f)
	{
		Angle -= 360.0f;
	}
	else if (Angle < -180.0f)
	{
		Angle += 360.0f;
	}

	return Angle;
}

float ConvertToRadian(float Degree)
{
	return 0.0f;
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

//Transform 행렬
FPMatrix MatrtixTranslation(const float& x, const float& y, const float& z)
{
	return FPMatrix(DirectX::XMMatrixTranslation(x, y, z));
}

FPMatrix MatrtixTranslation(const FPVector3& Location)
{
	return FPMatrix(DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z));
}

//Rotation 행렬

//RollPitchYaw
FPMatrix MatrixRotaionRollPitch(const float& PitchDegree, const float& YawDegree, const float& RollDegree)
{
	float PitchRadian = DirectX::XMConvertToRadians(PitchDegree);
	float YawRadian = DirectX::XMConvertToRadians(YawDegree);
	float RollRadian = DirectX::XMConvertToRadians(RollDegree);

	return FPMatrix(DirectX::XMMatrixRotationRollPitchYaw(PitchRadian, YawRadian, RollRadian));
}

//Quternion
FPMatrix MatrixRotationQuaternion(const FPQuaternion& quat)
{
	DirectX::XMVECTOR quaternion{ quat.x, quat.y, quat.z, quat.w };
	return FPMatrix(DirectX::XMMatrixRotationQuaternion(quaternion));
}

//Scale 행렬
FPMatrix MatrixScaling(const float& x, const float& y, const float& z)
{
	return FPMatrix(DirectX::XMMatrixScaling(x, y, z));
}

FPMatrix MatrixScaling(const FPVector3& Scale)
{
	DirectX::XMVECTOR ScaleVec{ Scale.x, Scale.y, Scale.z };
	return FPMatrix(DirectX::XMMatrixScalingFromVector(ScaleVec));
}


//View 행렬

//왼손 좌표계 용
FPMatrix MatrixLookAtLH(const FPVector3& eye, const FPVector3& target, const FPVector3& up)
{
	DirectX::XMVECTOR Eye{ eye.x, eye.y, eye.z };
	DirectX::XMVECTOR Target{ target.x, target.y, target.z };
	DirectX::XMVECTOR Up{ up.x, up.y, up.z };

	return FPMatrix(DirectX::XMMatrixLookAtLH(Eye, Target, Up));
}

FPMatrix MatrixLookToLH(const FPVector3& eye, const FPVector3& direction, const FPVector3& up)
{
	DirectX::XMVECTOR Eye{ eye.x, eye.y, eye.z };
	DirectX::XMVECTOR Direction{ direction.x, direction.y, direction.z };
	DirectX::XMVECTOR Up{ up.x, up.y, up.z };

	return FPMatrix(DirectX::XMMatrixLookToLH(Eye, Direction, Up));
}

//Projection 행렬
FPMatrix MatrixPerspectiveFovLH(const float& Fov, const float& Aspect, const float& Zn, const float& Zf)
{
	return FPMatrix(DirectX::XMMatrixPerspectiveFovLH(Fov, Aspect, Zn, Zf));
}