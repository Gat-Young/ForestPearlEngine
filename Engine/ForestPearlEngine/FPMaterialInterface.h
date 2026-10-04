#pragma once
#include "Object/Object.h"

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

		virtual void** GetVertexShaderPointer() = 0;
		virtual void** GetPixelShaderPointer() = 0;

		virtual void** GetVBLayoutPointer() = 0;

		virtual void** GetVertexConstPointer() = 0;
		virtual void** GetPixelConstPointer() = 0;

		virtual void UpdateMaterial(float DeltaTime) {};
};