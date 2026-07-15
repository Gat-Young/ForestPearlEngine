#include "Renderer.h"
#include <assert.h>
#include <iostream>

#include "../TextRenderList.h"
#include "../MeshRenderList.h"
#include "../GizmoRenderList.h"

//객체 해제/제거 매크로()
#ifndef SafeRelease
template<typename T> void _SafeRelease(T*& ptr)
{
	if (ptr) { ptr.Release(); ptr = nullptr; }
}
template<typename T> void _SafeDelete(T*& ptr)
{
	//if (ptr)					//조건 생략가.
	{ delete ptr;	ptr = nullptr; }
}
template<typename T> void _SafeDelArray(T*& ptr)
{
	//if (ptr)					//조건 생략가.
	{ delete[] ptr;	ptr = nullptr; }
}
#define SafeRelease		_SafeRelease
#define SafeDelete		_SafeDelete
#define SafeDelArray	_SafeDelArray
#endif


Renderer::Renderer(RenderingDevice& Device) : Device(Device)
{
}

HRESULT Renderer::InitializeRenderer(HWND hwnd)
{
	HRESULT hr = S_OK;

	hr = Device.SetDisplayMode(hwnd);
	assert(SUCCEEDED(hr) && "Failed To SetDisplayMode\n");

	hr = Device.CreateFactory();
	assert(SUCCEEDED(hr) && "Failed To Create DXGI Factory\n");

	hr = Device.CreateHighSpecDevice();
	assert(SUCCEEDED(hr) && "Failed To Create DXDevice\n");


	hr = Device.CreateSwapChain(hwnd);
	assert(SUCCEEDED(hr) && "스왑체인 생성 실패\n");

	hr = Device.SetBackBufferToRenderTargetView();
	assert(SUCCEEDED(hr) && "백버퍼 - 렌더타겟 설정 실패\n");

	hr = Device.CreateDepthStencil();
	assert(SUCCEEDED(hr) && "깊이-스텐실 버퍼 생성 실패\n");

	Device.OMSetRenderTargets();

	//뷰포트 설정
	Device.SetViewPort(0.0f, 0.0f, (FLOAT)Device.GetWidth(), (FLOAT)Device.GetHeight(), 0.0f, 1.0f);

	Device.GetDeviceInfo();
	
	FontBatch = Device.CreateSpriteBatch();
	Font = Device.CreateSpriteFont();

	shaderFactory = &ShaderFactory::GetShaderFactory();

	Device.RasterStateCreate();

	return hr;
}

void Renderer::ObjectRendering()
{
	std::vector<MeshRenderItem> RenderList = MeshRenderList::Get().GetRenderList();

	for (MeshRenderItem RenderItem : RenderList)
	{
		//렌더링 모드 전환
		Device.UpdateRSSetState(*RenderItem.isFill, *RenderItem.isCull);

		//Shader 설정
		Device.VSSetShader(RenderItem.VertexShader);
		Device.PSSetShader(RenderItem.PixelShader);

		//입력 레이아웃 설정
		Device.IASetInputLayout(RenderItem.VBLayout);

		//기하 위상 구조 설정
		Device.IASetPrimitiveTopology();

		//정점 버퍼 설정
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		Device.IASetVertexBuffers(0, 1, *RenderItem.VBIndex ,&stride, &offset);

		Device.Draw(*RenderItem.FaceSize*3, 0);
	}

}

void Renderer::UIRendering()
{

	std::vector<UIContextItem> RenderList = TextRenderList::Get().GetRenderList();

	FontBatch->Begin();

	for (UIContextItem UI : RenderList)
	{
		if (!(*(*(UI.active))))
		{
			continue;
		}
		XMFLOAT4 Color = { (UI.color->x), (UI.color->y), (UI.color->z), (UI.color->w) };
		XMFLOAT2 Position = { (float)(*(UI.x)), (float)(*(UI.y)) };
		Font->DrawString(FontBatch, UI.msg->c_str(), Position, XMLoadFloat4(&Color));
	}

	FontBatch->End();
};


HRESULT Renderer::Finalize()
{
	Device.RasterStateRelease();
	FontRelease();
	HRESULT hr = Device.DeviceFinalize();


	return hr;
}

RenderingDevice& Renderer::GetRenderingDevice()
{
	return Device;
}

void Renderer::ClearBackBuffer()
{
	Device.ClearBackBuffer();
}

void Renderer::RenderTargetPresent()
{
	Device.RenderTargetPresent();
}

void Renderer::FontRelease()
{
	SafeDelete(FontBatch);
	SafeDelete(Font);
}