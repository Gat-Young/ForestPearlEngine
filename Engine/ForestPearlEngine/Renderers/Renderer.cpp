#include "Renderer.h"
#include "DirectXMath.h"
#include <queue>
#include <assert.h>
#include <iostream>

#include "../FPGameInstance.h"
#include "../FPGameProjectSetting.h"

#include "../FPTextRenderList.h"
#include "../FPMeshRenderList.h"
#include "../FPCameraList.h"
#include "FPRenderingCommon.h"
#include "../FPViewPortClient.h"

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
struct MVPConstBuffer
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

	Device.GetDeviceInfo();
	
	FontBatch = Device.CreateSpriteBatch();
	Font = Device.CreateSpriteFont();

	shaderFactory = &ShaderFactory::GetShaderFactory();

	Device.CreateDepthStencilStateCreate();

	Device.RasterStateCreate();

	//256B의 ConstBuffer 생성
	Device.CreateObjectConstBuffer(256);
	Device.CreateVertexShaderConstBuffer(256);
	Device.CreatePixelShaderConstBuffer(256);
	Device.CreateVertexViewPortConstBuffer(256);
	Device.CreatePixelViewPortConstBuffer(256);

	return hr;
}

void Renderer::ObjectRendering()
{
	
	Device.OMSetDepthStencilState(ZEnable);


	//상수 버퍼 설정

	//Vertex Shader
	Device.ObjectSetConstantBuffers(0, 1);
	Device.VSSetConstantBuffers(1, 1);
	Device.VVPSetConstantBuffers(2, 1);

	//PixelShader
	Device.PSSetConstantBuffers(0, 1);
	Device.PVPSetConstantBuffers(1, 1);

	MVPConstBuffer MVPCB;

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
		
		//렌더링 모드 전환
		Device.UpdateRSSetState(*RenderItem.isFill, *RenderItem.isCull);

		//입력 레이아웃 설정
		Device.IASetInputLayout(*(RenderItem.VBLayout));

		//기하 위상 구조 설정
		Device.IASetPrimitiveTopology(*RenderItem.Topo);

		//Shader 설정
		Device.VSSetShader(*(RenderItem.VertexShader));
		Device.PSSetShader(*(RenderItem.PixelShader));

		//Shader ConstBuffer가 있다면 갱신
		if (*(RenderItem.VertexConst) != nullptr) Device.UpdateVertexShaderSubresource(0, *(RenderItem.VertexConst), 0, 0);
		if (*(RenderItem.PixelConst) != nullptr) Device.UpdatePixelShaderSubresource(0, *(RenderItem.PixelConst), 0, 0);

		//Camera Setting
		FPCameraList* CameraList = static_cast<FPCameraList*>(FPGameInstance::Get().GetCameraList());
		std::vector<CameraItem> CamList = CameraList->GetRenderList();

		for (CameraItem& CamItem : CamList)
		{
			if (!(*(CamItem.Active))) continue;

			std::vector<FPViewPort*> CamViewPorts;

			FPViewPortClient* ViewPortClient = static_cast<FPViewPortClient*>(FPGameInstance::Get().GetViewPortClient());

			CamViewPorts = ViewPortClient->GetViewPort(FPViewPortName::MainGameViewPort);
			if ((*(CamItem.TripleCam)))
			{
				CamViewPorts = ViewPortClient->GetViewPort(FPViewPortName::TripleWaySplitViewPort);
			}

			for (FPViewPort* CamViewPort : CamViewPorts)
			{

				Device.SetViewPort(CamViewPort->TopLeftX, CamViewPort->TopLeftY,
					CamViewPort->Width, CamViewPort->Height,
					CamViewPort->MinDepth, CamViewPort->MaxDepth);

				std::cout << CamViewPort->TopLeftX << " : " << CamViewPort->TopLeftY << " : "
					<< CamViewPort->Width << " : " << CamViewPort->Height << "\n";

				MVPCB.WorldMatrix = ((*(RenderItem.Scale)) * (*(RenderItem.Rotation)) * (*(RenderItem.Location))).Matrix;
				MVPCB.ViewMatrix = (*(CamItem.View)).Matrix;
				MVPCB.ProjMatrix = (*(CamItem.Projection)).Matrix;

				MVPCB.WVPMatrix = MVPCB.WorldMatrix * MVPCB.ViewMatrix * MVPCB.ProjMatrix;

				//Object 상수 버퍼 갱신
				Device.UpdateObjectSubresource(0, &MVPCB, 0, 0);


				//ViewPort Shader ConstBuffer가 있다면 갱신
				if ((CamViewPort->VertexConst) != nullptr) Device.UpdateVertexViewPortSubresource(0, CamViewPort->VertexConst, 0, 0);
				if ((CamViewPort->PixelConst) != nullptr) Device.UpdatePixelViewPortSubresource(0, CamViewPort->PixelConst, 0, 0);

				//정점 버퍼 설정
				UINT stride = *RenderItem.Stride;
				UINT offset = *RenderItem.Offset;

				int MeshSize = (RenderItem.VB)->size();
				for (int i = 0; i < MeshSize; ++i)
				{
					Device.IASetVertexBuffers(0, 1, (RenderItem.VB)->at(i), &stride, &offset);

					Device.Draw((RenderItem.VertexSize)->at(i), 0);
				}
			}

		}

		RenderQueue.pop();
	}

}

void Renderer::UIRendering()
{
	FPTextRenderList* TextRenderList = static_cast<FPTextRenderList*>(FPGameInstance::Get().GetTextRenderList());
	std::vector<UIContextItem> RenderList = TextRenderList->GetRenderList();

	std::vector<FPViewPort*> CamViewPorts;

	FPViewPortClient* ViewPortClient = static_cast<FPViewPortClient*>(FPGameInstance::Get().GetViewPortClient());

	CamViewPorts = ViewPortClient->GetViewPort(FPViewPortName::UIViewPort);

	for (FPViewPort* CamViewPort : CamViewPorts)
	{
		Device.SetViewPort(CamViewPort->TopLeftX, CamViewPort->TopLeftY,
			CamViewPort->Width, CamViewPort->Height,
			CamViewPort->MinDepth, CamViewPort->MaxDepth);
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
	}
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

//RenderTarget 재생성 (임시)
void Renderer::ResizeRenderTarget()
{
	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	
	//기존 렌더 타겟 바인딩 해제
	Device.OMResetRenderTargets();

	//기존 RenderTargetView / Depth 관련 객체 해제
	Device.ResetRTVandDepthObj();

	//DisplayMode 재설정
	Device.DisplayModeSize(GameProjectSetting->GetDisplayWidth(), GameProjectSetting->GetDeisplayHeight());

	//Swapchain BackBuffer Resize
	Device.ResizeSwapChainBuffer(GameProjectSetting->GetDisplayWidth(), GameProjectSetting->GetDeisplayHeight());

	//새 BackBuffer로 RTV 생성
	HRESULT hr = S_OK;
	hr = Device.SetBackBufferToRenderTargetView();
	assert(SUCCEEDED(hr) && "백버퍼 - 렌더타겟 설정 실패\n");

	//새 Depth Buffer생성
	hr = Device.CreateDepthStencil();
	assert(SUCCEEDED(hr) && "깊이-스텐실 버퍼 생성 실패\n");

	Device.OMSetRenderTargets();
}