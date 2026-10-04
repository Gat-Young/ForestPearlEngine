#pragma once
#include <string>
#include "FPMaterialInterface.h"

class FPMaterial : public FPMaterialInterface
{
	protected:
		void* VertexShader = nullptr;
		void* PixelShader = nullptr;

		//정점 셰이더 컴파일 코드 개체
		void* VSCode = nullptr;

		//픽셀 셰이더 컴파일 코드 개체
		void* PSCode = nullptr;

		//정점 입력구조 Input Layout
		void* VBLayout = nullptr;

		//Material 상수 버퍼
		void* VertextConst = nullptr;
		void* PixelConst = nullptr;
		

	public:
		FPMaterial();
		void SetVertexShader(std::string VertexShaderPath) override;
		void SetPixelShader(std::string PixelShaderPath) override;

		void** GetVertexShaderPointer() override;
		void** GetPixelShaderPointer() override;

		void** GetVBLayoutPointer() override;

		void** GetVertexConstPointer() override;
		void** GetPixelConstPointer() override;

		virtual void UpdateMaterial(float DeltaTime) {};

};