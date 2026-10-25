#pragma once
#include "FPSceneComponent.h"
#include "FPLightRenderList.h"

class FPLightComponentBase : public FPSceneComponent
{
	protected:
		bool isActive = true;

		struct alignas(16) FPLightConstantBuffer
		{
			FPVector3 Direction = { 0.0f, 1.0f, 0.0f };
			float Range = 1000.0f;
			FPVector4 Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			FPVector4 Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
			FPVector4 Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
			FPVector4 Attenuation = { 1.0f, 1.0f, 1.0f, 1.0f };
			FPVector4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			unsigned int LightType = LightType_None;
		};

		FPLightConstantBuffer LightConstantBuffer;

		//상수 버퍼 정보
		FPConstantBufferRenderData VertexConstantBuffer;
		FPConstantBufferRenderData PixelConstantBuffer;

		FPLightRenderItem* RenderItem = nullptr;

		virtual void RegistLightRenderList();

	public:
		FPLightComponentBase(FPActor* Owner);
		~FPLightComponentBase();
		void SetActive(bool Active) { this->isActive = Active; }
		void SetDirection(FPVector3& Dir);
		void SetRange(float Ran);
		void SetDiffuse(FPVector4& Diffuse);
		void SetAmbient(FPVector4& Ambient);
		void SetLightColor(FPVector4& LightColor);
		void SetSpecular(FPVector4& Specular);
		void SetAttenuation(FPVector4& Attenuation);
};