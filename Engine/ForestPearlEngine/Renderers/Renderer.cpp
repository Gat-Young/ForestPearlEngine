#include "Renderer.h"
#include "DirectXMath.h"
#include <queue>
#include <assert.h>
#include <iostream>

#include "../FPGameInstance.h"

#include "../FPTextRenderList.h"
#include "../FPMeshRenderList.h"
#include "../FPCameraList.h"
#include "FPRenderingCommon.h"

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

//정점 구조체
struct VERTEX
{
	float x, y, z;		//좌표 Position
	float r, g, b, a;	//색상 Diffuse Color
};

//DirectX Math 타입
// XMMATRIX		<행렬		: 16바이트 정렬,	SIMD 버전,	전역/지역 변수용,		Register Type
// XMFLOAT4X4	<행렬		: 일반 버전,		SIMD 미지원, Class 데이터 저장용,	Storage Type
// XMVECTOR		<4성분 백터	: 16바이트 정렬,	SIMD 버전,	전역/지역 변수용,		RegisterType
// XMFLOAT4		<4성분 벡터	: 일반 버전,		SIMD 미지원,	Class 데이터 저장용,	Storage Type
// XMFLOAT4		<3성분 벡터	: 일반 버전,		SIMD 미지원,	Class 데이터 저장용,	Storage Type
// XMFLOAT4		<2성분 벡터	: 일반 버전,		SIMD 미지원,	Class 데이터 저장용,	Storage Type

//색상 타입 2가지
// XMCOLOR		<색상,	4성분 (r, g, b, a)	[정수형 0 ~ 255]
// XMFLOAT4		<색상,	4성분 (r, g, b, a)	[실수형 0 ~ 1.0]
// 

//상수 버퍼용 구조체 : 셰이더 내부 연산에 사용될 데이터들
struct ConstBuffer
{
	XMMATRIX WorldMatrix;
	XMMATRIX ViewMatrix;
	XMMATRIX ProjMatrix;
	XMMATRIX WVPMatrix;
};



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

	Device.CreateDepthStencilStateCreate();

	Device.RasterStateCreate();

	Device.CreateConstBuffer(sizeof(ConstBuffer));

	return hr;
}

void Renderer::ObjectRendering()
{
	
	Device.OMSetDepthStencilState(ZEnable);

	ConstBuffer cb;

	//Camera Setting
	FPCameraList* CameraList = static_cast<FPCameraList*>(FPGameInstance::Get().GetCameraList());
	std::vector<CameraItem> CamList = CameraList->GetRenderList();
	
	XMMATRIX ViewMatrix = XMMatrixIdentity();
	XMMATRIX ProjectionMatrix = XMMatrixIdentity();

	for (CameraItem& CamItem : CamList)
	{
		if (!(*(CamItem.Active))) continue;

		//View 행렬
		XMFLOAT4X4 xmView;
		XMVECTOR eye, lookat, up;
		eye = XMVectorSet(CamItem.Location->x, CamItem.Location->y, CamItem.Location->z, 1);
		lookat = XMVectorSet(CamItem.LookAt->x, CamItem.LookAt->y, CamItem.LookAt->z, 1);
		up = XMVectorSet(CamItem.Up->x, CamItem.Up->y, CamItem.Up->z, 1);
		XMStoreFloat4x4(&xmView, XMMatrixLookAtLH(eye, lookat, up));
		ViewMatrix = XMLoadFloat4x4(&xmView);
		

		//Projection 행렬
		XMFLOAT4X4 xmProj;
		XMStoreFloat4x4(&xmProj, XMMatrixPerspectiveFovLH(XMConvertToRadians(*CamItem.Fov), *CamItem.Aspect, *CamItem.Zn, *CamItem.Zf));
		ProjectionMatrix = XMLoadFloat4x4(&xmProj);

	}

	cb.ViewMatrix = ViewMatrix;
	cb.ProjMatrix = ProjectionMatrix;

	//Object Draw
	FPMeshRenderList* MeshRenderList = static_cast<FPMeshRenderList*>(FPGameInstance::Get().GetMeshRenderList());
	std::vector<RenderItem> RenderList = MeshRenderList->GetRenderList();
	
	//Priority 값이 큰 걸 우선해서 그림
	auto Compare = [](const RenderItem& Left, const RenderItem& Right)
		{
			return *(Left.Priority) < *(Right.Priority);
		};

	std::priority_queue<RenderItem, std::vector<RenderItem>, decltype(Compare)> RenderQueue(Compare);

	for (RenderItem& RenderItem : RenderList)
	{
		RenderQueue.push(RenderItem);
	}

	while(!RenderQueue.empty())
	{
		const RenderItem& RenderItem = RenderQueue.top();

		if (!(*(RenderItem.Active)))
		{
			RenderQueue.pop();
			continue;
		}
		XMMATRIX TransformMatrix = XMMatrixIdentity();

		//스케일 처리
		XMFLOAT4X4 xmScale;
		XMStoreFloat4x4(&xmScale, XMMatrixScaling(RenderItem.Scale->x, RenderItem.Scale->y, RenderItem.Scale->z));
		XMMATRIX Scale = XMLoadFloat4x4(&xmScale);

		//회전 처리
		XMFLOAT4X4 xmQuaternionRotation;
		XMVECTOR xmQuaternion = { RenderItem.Rotation->x, RenderItem.Rotation->y, RenderItem.Rotation->z, RenderItem.Rotation->w };
		XMStoreFloat4x4(&xmQuaternionRotation, XMMatrixRotationQuaternion(xmQuaternion));
		XMMATRIX Rotation = XMLoadFloat4x4(&xmQuaternionRotation);

		//이동 처리
		XMFLOAT4X4 xmPosition;
		XMStoreFloat4x4(&xmPosition, XMMatrixTranslation(RenderItem.Location->x, RenderItem.Location->y, RenderItem.Location->z));
		XMMATRIX Position = XMLoadFloat4x4(&xmPosition);

		//모델링 행렬 SRT
		TransformMatrix = Scale * Rotation * Position;

		cb.WorldMatrix = TransformMatrix;
		cb.WVPMatrix = cb.WorldMatrix * cb.ViewMatrix * cb.ProjMatrix;

		//상수 버퍼 갱신
		Device.UpdateSubresource(0, &cb, 0, 0);
		//렌더링 모드 전환
		Device.UpdateRSSetState(*RenderItem.isFill, *RenderItem.isCull);

		//Shader 설정
		Device.VSSetShader(RenderItem.VertexShader);
		Device.PSSetShader(RenderItem.PixelShader);

		//상수 버퍼 설정
		Device.VSSetConstantBuffers(0, 1);

		//입력 레이아웃 설정
		Device.IASetInputLayout(RenderItem.VBLayout);

		//기하 위상 구조 설정
		Device.IASetPrimitiveTopology(*RenderItem.Topo);

		//정점 버퍼 설정
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;

		int MeshSize = (RenderItem.VBIndex)->size();
		for (int i = 0; i < MeshSize; ++i)
		{
			Device.IASetVertexBuffers(0, 1, (RenderItem.VBIndex)->at(i), &stride, &offset);

			Device.Draw((RenderItem.VertexSize)->at(i), 0);
		}

		RenderQueue.pop();
	}

}

void Renderer::UIRendering()
{
	FPTextRenderList* TextRenderList = static_cast<FPTextRenderList*>(FPGameInstance::Get().GetTextRenderList());
	std::vector<UIContextItem> RenderList = TextRenderList->GetRenderList();

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