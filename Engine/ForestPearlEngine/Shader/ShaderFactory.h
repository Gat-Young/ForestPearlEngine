#pragma once
#include <d3d11.h>
#include <dxgi1_6.h>
#pragma comment(lib, "D3D11")
#pragma comment(lib, "dxgi.lib")

#include <Windows.h>
#include <wrl/client.h>
#include <tchar.h>
#include <DirectXMath.h>
#pragma comment(lib, "DirectXTK.lib")
using namespace DirectX;

class ShaderFactory
{

	private:
		HRESULT ShaderCompile(const TCHAR* FileName, const CHAR* EntryPoint, const CHAR* ShaderModel, ID3DBlob** ppCode);

		ShaderFactory() = default;

	public:
		static ShaderFactory& GetShaderFactory();

		//목적 파일 로드
		HRESULT VertexShaderLoad(const TCHAR* Objectname, void** ppVS, void** ppCode = NULL);
		HRESULT PixelShaderLoad(const TCHAR* Objectname, void** ppPS, void** ppCode = NULL);
		
		//D3D Compiler 사용
		HRESULT VertexShaderLoad(const TCHAR* filename, const CHAR* entry, const CHAR* target, void** ppVS, void** ppCode = NULL);
		HRESULT PixelShaderLoad(const TCHAR* filename, const CHAR* entry, const CHAR* target, void** ppPS, void** ppCode = NULL);


		HRESULT CreateInputLayout(void* InVSCode, void** ReturnLayout);
};