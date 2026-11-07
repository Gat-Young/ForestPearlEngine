#pragma once
#include "FPSceneComponent.h"

class FPSceneCaptureComponent : public FPSceneComponent
{
	protected:
		//기본 Camera 속성
		
		//투영 변환 용
		float Fov = DegToRad(45.0f);							//시야각			
		float Aspect = 960.0f / 600.0f;				//가로:세로 비율
		float Zn = 1.0f;							//근평면 거리
		float Zf = 300.0f;							//원평면 거리

		FPMatrix ViewMatrix;
		FPMatrix ProjectionMatrix;

		bool Active = true;			//카메라 사용 여부



	public:
		virtual void RegistSceneCaptureComponent() = 0;
		virtual void SetActive() = 0;
};