#pragma once
#include <string>
#include "FPMaterialInterface.h"

class FPMaterial : public FPMaterialInterface
{
	protected:
		std::string VertexShaderPath;
		std::string PixelShaderPath;

		//상수 버퍼 정보
		FPConstantBufferRenderData VertexConstantBuffer;
		FPConstantBufferRenderData PixelConstantBuffer;

	public:
		FPMaterial();
		void SetVertexShader(std::string VertexShaderPath) override;
		void SetPixelShader(std::string PixelShaderPath) override;
		std::string* GetVertexShader() override;
		std::string* GetPixelShader() override;

		FPConstantBufferRenderData* GetVertexConstantBufferRenderData() override;
		FPConstantBufferRenderData* GetPixelConstantBufferRenderData() override;

		virtual void UpdateMaterial(float DeltaTime) {};

};