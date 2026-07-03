#pragma once

#include <d3d11.h>
#include <dxgi1_6.h>
#pragma comment(lib, "D3D11")

#include <Windows.h>
#include <wrl/client.h>


class Renderer
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	private:
		//D3D Factory
		ComPtr<IDXGIFactory2> Factory = NULL;

		//D3D 장치 인터페이스
		ComPtr<ID3D11Device> Device = NULL;
		ComPtr<ID3D11DeviceContext> DeviceContext = NULL;

		//D3D 스왑체인 인터페이스
		ComPtr<IDXGISwapChain> SwapChain = NULL;
		ComPtr<ID3D11RenderTargetView> RenderTargetView = NULL;

		DXGI_MODE_DESC1 DisplayMode;

		//D3D Feature Level 확인
		D3D_FEATURE_LEVEL FeatureLevels[2] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL ActualLevel;
	public:
		Renderer();

		HRESULT InitializeRenderer(HWND hwnd);
};