#pragma once
#include "FPSceneCaptureComponent.h"

class FPSceneCaptureComponent2D : public FPSceneCaptureComponent
{
	public:
		void RegistSceneCaptureComponent() override;
		void SetActive(bool Active) override;
		void SetRenderTarget(std::string& RenderTarget) override;
		void SetCaptureEveryFrame(bool Active) override;
		void CaptureScene() override;
};