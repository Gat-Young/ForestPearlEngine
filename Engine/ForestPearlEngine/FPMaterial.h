#pragma once
#include <string>
#include "FPMaterialInterface.h"
#include "FPTexture.h"
#include <unordered_map>

class FPMaterial : public FPMaterialInterface
{

	protected:
		std::string VertexShaderPath;
		std::string PixelShaderPath;

		//Two Side 여부 <Two Side 체크시 해당 메시는 Cull 하지 않으며 Back Normal을 사용> (기본값 : False)
		bool TwoSided = false;

		std::string BlendState = "BS_ADD";
		FPMaterialDetail Detail = FPMaterialDetail::Opaque;
		bool bIsSky = false;
		bool bUsePlanarForwardReflections = false;

		// 셰이더 분석 결과는 FPMaterial이 보관하며 GPU 작업은 하지 않음
		FPShaderReflectionData ReflectionData;

		//상수 버퍼 정보
		FPConstantBufferRenderData VertexConstantBuffer;
		FPConstantBufferRenderData PixelConstantBuffer;

		bool HasTexture(int Slot);

	public:
		FPMaterial();
		void SetVertexShader(std::string VertexShaderPath) override;
		void SetPixelShader(std::string PixelShaderPath) override;
		std::string* GetVertexShader() override;
		std::string* GetPixelShader() override;

		bool SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const std::vector<float>& Values) override;
		bool SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const FPVector4& Value) override;
		bool SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const FPVector3& Value) override;
		bool SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const FPVector2& Value) override;
		bool SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const float& Value) override;
		bool GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, std::vector<float>& Out) const override;
		bool GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, FPVector4& Out) const override;
		bool GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, FPVector3& Out) const override;
		bool GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, FPVector2& Out) const override;
		bool GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, float& Out) const override;

		bool SetNamedTexture(FPShaderType ShaderType, const std::string& HLSLTexture, std::string AssetName, FPTextureType TextureType, unsigned int ArrayIndex) override;
		bool SetNamedSampler(FPShaderType ShaderType, const std::string& HLSLSampler, const std::string& SamplerType, unsigned int ArrayIndex) override;

		bool SetParameterElement(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, unsigned int ArrayIndex, const std::vector<float>& Values) override;
		bool GetParameterElement(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, unsigned int ArrayIndex, std::vector<float>& OutValues) const override;

		FPShaderReflectionData* GetReflectionData() override;
		FPConstantBufferRenderData* GetVertexConstantBufferRenderData() override;
		FPConstantBufferRenderData* GetPixelConstantBufferRenderData() override;

		void SetTwoSided(bool isTwoSide) override;
		bool* GetTwoSided()override;

		void SetBlendState(std::string BlendState) override;
		void SetDetail(FPMaterialDetail Detail) override;
		std::string* GetBlendState() override;
		FPMaterialDetail* GetDetail() override;
		void SetIsSky(bool bIsSky) override;
		bool* GetIsSky() override;
		void SetUsePlanarForwardReflections(bool bUse) override;
		bool* GetUsePlanarForwardReflections() override;

		//상속 받은 구현체에서 반드시 Super를 호출 할 것
		virtual void UpdateMaterial(float DeltaTime) override;

};