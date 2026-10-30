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
		virtual void SetVertexShader(std::string VertexShaderPath) = 0;
		virtual void SetPixelShader(std::string PixelShaderPath) = 0;
		virtual std::string* GetVertexShader() = 0;
		virtual std::string* GetPixelShader() = 0;
		virtual FPConstantBufferRenderData* GetVertexConstantBufferRenderData() = 0;
		virtual FPConstantBufferRenderData* GetPixelConstantBufferRenderData() = 0;
		virtual FPShaderReflectionData* GetReflectionData() = 0;

		// 기존 인터페이스의 하위 클래스와 호환되는 선택적 Parameter 함수
		virtual bool SetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, const std::vector<float>&) = 0;
		virtual bool SetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, const FPVector4&) = 0;
		virtual bool SetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, const FPVector3&) = 0;
		virtual bool SetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, const FPVector2&) = 0;
		virtual bool SetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, const float&) = 0;

		virtual bool GetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, std::vector<float>&) const = 0;
		virtual bool GetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, FPVector4&) const = 0;
		virtual bool GetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, FPVector3&) const = 0;
		virtual bool GetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, FPVector2&) const = 0;
		virtual bool GetParameter(FPShaderType, const std::string& ConstantBufferName, const std::string& ParameterName, float&) const = 0;
		// 배열 Parameter의 특정 인덱스만 변경하며 기존 하위 클래스는 유지
		virtual bool SetParameterElement(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, unsigned int ArrayIndex, const std::vector<float>& Values) = 0;
		// 배열 Parameter의 특정 원소를 조회
		virtual bool GetParameterElement(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, unsigned int ArrayIndex, std::vector<float>& OutValues) const = 0;

		virtual bool SetNamedTexture(FPShaderType ShaderType, const std::string& HLSLTexture, std::string AssetName, FPTextureType TextureType, unsigned int ArrayIndex = 0) = 0;
		virtual bool SetNamedSampler(FPShaderType ShaderType, const std::string& HLSLSampler, FPSamplerType SamplerType, unsigned int ArrayIndex = 0) = 0;
		//TwoSide
		virtual void SetTwoSided(bool isTwoSide) = 0;
		virtual bool* GetTwoSided() = 0;
		virtual void UpdateMaterial(float DeltaTime) {};
};