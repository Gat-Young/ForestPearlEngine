#pragma once
#include <string>
#include "FPMaterialInterface.h"
#include "FPTexture2D.h"
#include <unordered_map>

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

		//Texture List <Slot, Data>
		std::unordered_map<int, FPTexture2D> TextureList;
		std::unordered_map<int, FPTextureItem> TextureItmeList;

		//공용 마테리얼 Vertex 버퍼
		FPMaterialVertexConstantBuffer MaterialVertexConstantBuffer;

		//상수 버퍼 정보
		FPConstantBufferRenderData VertexConstantBuffer;
		FPConstantBufferRenderData PixelConstantBuffer;

		bool HasTexture(int Slot);

	public:
		FPMaterial();
		//Texture 운용 함수
		void SetTexture(int Slot, std::string& TextureName, FPSamplerType SamplerType) override;
		void SetTexture(int Slot, std::string& TextureName) override;
		void SetTextureSampler(int Slot, FPSamplerType SamplerType) override;
		void AddTexture(int Slot) override;
		void DeleteTexture(int Slot) override;

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

		std::unordered_map<int, FPTextureItem>* GetTextureList() override;

		FPConstantBufferRenderData* GetVertexConstantBufferRenderData() override;
		FPConstantBufferRenderData* GetPixelConstantBufferRenderData() override;

		//상속 받은 구현체에서 반드시 Super를 호출 할 것
		virtual void UpdateMaterial(float DeltaTime) override;

};