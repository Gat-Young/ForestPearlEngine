#pragma once
#include <string>

class FPMaterial
{
	private:
		void* VertexShader = nullptr;
		void* PixelShader = nullptr;

		//정점 셰이더 컴파일 코드 개체
		void* VSCode = nullptr;

		//픽셀 셰이더 컴파일 코드 개체
		void* PSCode = nullptr;

		//정점 입력구조 Input Layout
		void* VBLayout = nullptr;

		//Material 속성
		

	public:
		FPMaterial();
		void SetVertexShader(std::string VertexShaderPath);
		void SetPixelShader(std::string PixelShaderPath);

		void* GetVertexShaderPointer();
		void* GetPixelShaderPointer();

		void* GetVBLayoutPointer();

};