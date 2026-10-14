#pragma once
#include <string>
#include "FPMaterialInterface.h"
#include "FPTexture2D.h"

class FPMaterial : public FPMaterialInterface
{
	struct alignas(16) FPMaterialVertexConstantBuffer
	{
		FPVector4 BaseColor;
		FPVector4 Diffuse;
		FPVector4 Ambient;
	};

	protected:
		std::string VertexShaderPath;
		std::string PixelShaderPath;

		//Texture List <최대 128개>
		std::vector<FPTexture2D> TextureList;

		//공용 마테리얼 Vertex 버퍼
		FPMaterialVertexConstantBuffer MaterialVertexConstantBuffer;

		//상수 버퍼 정보
		FPConstantBufferRenderData VertexConstantBuffer;
		FPConstantBufferRenderData PixelConstantBuffer;

	public:
		FPMaterial();
		//Texture 운용 함수
		void SetTexture(int Slot, std::string& TextureName, FPSamplerType SamplerType) override;
		void SetTexture(int Slot, std::string& TextureName) override;
		void SetTextureSampler(int Slot, FPSamplerType SamplerType) override;

		void SetVertexShader(std::string VertexShaderPath) override;
		void SetPixelShader(std::string PixelShaderPath) override;
		std::string* GetVertexShader() override;
		std::string* GetPixelShader() override;

		void SetBaseColor(FPVector4& BaseColor) override;
		void SetDiffuse(FPVector4& Diffuse) override;
		void SetAmbient(FPVector4& Ambient) override;

		FPVector4* GetBaseColor() override;
		FPVector4* GetDiffuse() override;
		FPVector4* GetAmbient() override;

		void GetTextureList(std::vector<FPTextureItem>& TextureRenderList) override;

		FPConstantBufferRenderData* GetVertexConstantBufferRenderData() override;
		FPConstantBufferRenderData* GetPixelConstantBufferRenderData() override;

		//상속 받은 구현체에서 반드시 Super를 호출 할 것
		virtual void UpdateMaterial(float DeltaTime) override;

};