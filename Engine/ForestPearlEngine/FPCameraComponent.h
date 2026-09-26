#pragma once
#include "FPPrimitiveComponent.h"
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPCameraList.h"

class FPCameraComponent : public FPPrimitiveComponent
{
	public:
		//카메라 속성
		FPVector3 LookAt = {0.0f, 0.0f, 1.0f};		//바라보는 곳. 위치
		FPVector3 Up = {0.0f, 1.0f, 0.0f};			//카메라 상방 벡터

		//투영 변환 용
		float Fov = DegToRad(45.0f);							//시야각			
		float Aspect = 960.0f / 600.0f;				//가로:세로 비율
		float Zn = 1.0f;							//근평면 거리
		float Zf = 100.0f;							//원평면 거리

		FPMatrix ViewMatrix;
		FPMatrix ProjectionMatrix;

		bool Active = true;			//카메라 사용 여부
		bool TripleCame = false;	//(임시) 3-분할 카메라 사용 여부

		CameraItem* CamItem = nullptr;

	public:
		FPCameraComponent(FPActor* Owner);
		~FPCameraComponent();
		void RegistCamera();
		void Tick() override;

		void OnTripleCam();
};