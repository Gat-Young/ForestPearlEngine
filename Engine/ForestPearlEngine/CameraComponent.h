#pragma once
#include "../ForestPearlEngine/Define/FPMath.h"
#include "CameraList.h"

class CameraComponent
{
	public:
		//카메라의 위치
		FPVector3 Position;
		FPVector3 Rotation;
		FPVector3 Scale;

		//카메라 속성
		FPVector3 LookAt;		//바라보는 곳. 위치
		FPVector3 Up;			//카메라 상방 벡터

		//투영 변환 용
		float Fov;				//시야각			
		float Aspect;			//가로:세로 비율
		float Zn;				//근평면 거리
		float Zf;				//원평면 거리

		bool Active;			//카메라 사용 여부

		CameraItem* CamItem = nullptr;

	public:
		CameraComponent() = default;
		~CameraComponent();
		void RegistCamera();
};