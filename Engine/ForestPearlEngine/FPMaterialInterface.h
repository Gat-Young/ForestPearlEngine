#pragma once
#include "Object/Object.h"
#include "Renderers/FPRenderingCommon.h"
#include <unordered_map>

////////////////////////////////////////////////
//
//	Material Instance와 Material의 공통 인터페이스
//
//
class FPMaterialInterface : public FPObject
{
	public:
		//Texture 운용 함수
		virtual void SetTexture(int Slot, std::string& TextureName, FPSamplerType SamplerType) = 0;
		virtual void SetTexture(int Slot, std::string& TextureName) = 0;
		virtual void SetTextureSampler(int Slot, FPSamplerType SamplerType) = 0;
		virtual std::unordered_map<int, FPTextureItem>* GetTextureList() = 0;
		virtual void AddTexture(int Slot) = 0;
		virtual void DeleteTexture(int Slot) = 0;

		virtual void SetVertexShader(std::string VertexShaderPath) = 0;
		virtual void SetPixelShader(std::string PixelShaderPath) = 0;
		virtual std::string* GetVertexShader() = 0;
		virtual std::string* GetPixelShader() = 0;
		virtual FPConstantBufferRenderData* GetVertexConstantBufferRenderData() = 0;
		virtual FPConstantBufferRenderData* GetPixelConstantBufferRenderData() = 0;
		
		//공용 마테리얼 운용 함수
		virtual void SetBaseColor(FPVector4& BaseColor) = 0;
		virtual void SetDiffuse(FPVector4& Diffuse) = 0;
		virtual void SetAmbient(FPVector4& Ambient) = 0;

		virtual FPVector4* GetBaseColor() = 0;
		virtual FPVector4* GetDiffuse() = 0;
		virtual FPVector4* GetAmbient() = 0;

		virtual void UpdateMaterial(float DeltaTime) {};
};