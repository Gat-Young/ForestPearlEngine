#pragma once
#include "FPLightComponentBase.h"
#include "FPSkyLightRenderList.h"

class FPSkyLightComponent : public FPLightComponentBase
{
	private:
		bool isActive = true;

		bool RealTimeCapture = false;		//실시간 환경광 캡처

		std::string CubeMap;				//현재는 Sphere Map으로 우선 테스트

		FPVector4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float LightIntensity = 1.0f;

		FPSkyLightRenderItem* RenderItem = nullptr;

		void RegistSkyLightRenderList();

	public:
		FPSkyLightComponent(FPActor* Owner);
		~FPSkyLightComponent();

		void SetActive(bool Active) { this->isActive = Active; }
		void SetRealTimeCapture(bool RealTimeCapture);
		void SetLightIntnesity(float Intensity);
		void SetLightColor(FPVector4 Color);
		void SetCubeMap(std::string CubeMap);
};
