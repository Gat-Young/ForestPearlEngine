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
#include "SpriteFont.h"
#include "SpriteBatch.h"
using namespace DirectX;

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
		ComPtr<IDXGISwapChain1> SwapChain = NULL;
		ComPtr<ID3D11RenderTargetView> RenderTargetView = NULL;

		DXGI_MODE_DESC1 DisplayMode;

		//D3D Feature Level 확인
		D3D_FEATURE_LEVEL FeatureLevels[2] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL ActualLevel;

		const TCHAR* StrFeatureLevel = _T("N/A");

		XMFLOAT4 BackGroundColor = { 0, 0.125f, 0.3f, 1 };
		
		//폰트 정보
		SpriteBatch* FontBatch = nullptr;
		SpriteFont* Font = nullptr;

		//뷰포트 세팅
		void SetViewPort(float TopLeftX, float TopLeftY, float Width, float Height, float MinDepth, float MaxDepth);
		
		//폰트 생성
		HRESULT FontCreate();

		//폰트 해제
		void FontRelease();

		//장치 정보 획득
		void GetDeviceInfo();

		//장치 기능레벨 확인
		void GetFeatureLevel();


	public:
		Renderer();

		HRESULT InitializeRenderer(HWND hwnd);

		void ClearBackBuffer();

		void ObjectRendering();

		void UIRendering();

		void RenderTargetPresent();

		HRESULT Finalize();
};