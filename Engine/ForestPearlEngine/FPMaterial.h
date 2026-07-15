#pragma once
#include "tchar.h"

class FPMaterial
{
	private:
		void* VertexShader = nullptr;
		void* PixelShader = nullptr;

		//정점 셰이더 컴파일 코드 개체
		void* VSCode = nullptr;

		//픽셀 셰이더 컴파일 코드 개체
		void* PSCode = nullptr;

		//셰이더 파일 이름
		const TCHAR* Filename = _T("../../Engine/ForestPearlEngine/Shader/fx/Demo.fx");

		//정점 입력구조 Input Layout
		void* VBLayout = nullptr;

	public:
		FPMaterial();
		void* GetVertexShaderPointer();
		void* GetPixelShaderPointer();
		void* GetVBLayoutPointer();

};