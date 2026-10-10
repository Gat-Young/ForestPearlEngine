#pragma once
#include "Object/Object.h"
#include "Renderers/FPRenderingCommon.h"

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
		
		//공용 마테리얼 운용 함수
		virtual void SetBaseColor(FPVector4& BaseColor) = 0;
		virtual void SetDiffuse(FPVector4& Diffuse) = 0;
		virtual void SetAmbient(FPVector4& Ambient) = 0;

		virtual void UpdateMaterial(float DeltaTime) {};
};