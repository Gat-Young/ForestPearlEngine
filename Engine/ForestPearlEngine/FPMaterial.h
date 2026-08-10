#pragma once
#include <string>

class FPMaterial
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
		void SetVertexShader(std::string VertexShaderPath);
		void SetPixelShader(std::string PixelShaderPath);

		void* GetVertexShaderPointer();
		void* GetPixelShaderPointer();

		void* GetVBLayoutPointer();

		void* GetVertexConstPointer();
		void* GetPixelConstPointer();

		virtual void UpdateMaterial(float DeltaTime);

};