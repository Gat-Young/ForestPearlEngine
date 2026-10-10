#pragma once
#include "FPLightComponent.h"

class FPDirectionalLightComponent : public FPLightComponent
{
	protected:

		struct alignas(16) FPDirectionalLightConstantBuffer
		{
			FPVector3 Direction = {0.0f, -1.0f, 0.0f};
			float Range = 1000.0f;
			FPVector4 Diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
			FPVector4 Ambient = {0.2f, 0.2f, 0.2f, 1.0f};
		};

		FPDirectionalLightConstantBuffer DirectionalLightConstantBuffer;

		//상수 버퍼 정보
		FPConstantBufferRenderData VertexConstantBuffer;
		FPConstantBufferRenderData PixelConstantBuffer;

		virtual void RegistLightRenderList() override;

	public:
		FPDirectionalLightComponent(FPActor* Ownder);
		~FPDirectionalLightComponent();

		void SetDirection(FPVector3& Dir);
		void SetRange(float Ran);
		void SetDiffuse(FPVector4& Diffuse);
		void SetAmbient(FPVector4& Ambient);
};
