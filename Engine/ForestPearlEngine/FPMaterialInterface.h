#pragma once
#include "Object/Object.h"
#include "Renderers/FPConstantBufferCommon.h"

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
		virtual FPConstantBufferRenderData* GetVertexConstantBufferRenderData() = 0;
		virtual FPConstantBufferRenderData* GetPixelConstantBufferRenderData() = 0;

		virtual void UpdateMaterial(float DeltaTime) {};
};