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

class Shader
{

	private:
		//DX Device 참조
		ID3D11Device& Device;

		//셰이더 객체
		ID3D11VertexShader* VertexShader = nullptr;
		ID3D11PixelShader* PixelShader = nullptr;

		//정점 셰이더 컴파일 코드 개체
		ID3DBlob* VSCode = nullptr;

		//픽셀 셰이더 컴파일 코드 개체
		ID3DBlob* PSCode = nullptr;

		//셰이더 파일 이름
		const TCHAR* Filename = _T("../../Engine/ForestPearlEngine/Shader/fx/Demo.fx");

		HRESULT VertexShaderLoad(const TCHAR* fxname, const CHAR* entry, const CHAR* target, ID3D11VertexShader** ppVS, ID3DBlob** ppCode = NULL);
		HRESULT PixelShaderLoad(const TCHAR* fxname, const CHAR* entry, const CHAR* target, ID3D11PixelShader** ppPS, ID3DBlob** ppCode = NULL);
		HRESULT ShaderCompile(const TCHAR* FileName, const CHAR* EntryPoint, const CHAR* ShaderModel, ID3DBlob** ppCode);

	public:
		Shader(ID3D11Device& Device);
		void 	ShaderCreate();
		void 	ShaderUpdate();
		void 	ShaderRelease();

		HRESULT	ShaderLoad();

		//VertexShader Set
		void VertexShaderSet(ID3D11DeviceContext& DXDC) { DXDC.VSSetShader(VertexShader, nullptr, 0); };
		//PixelShader Set
		void PixelShaderSet(ID3D11DeviceContext& DXDC) { DXDC.PSSetShader(PixelShader, nullptr, 0); };

};