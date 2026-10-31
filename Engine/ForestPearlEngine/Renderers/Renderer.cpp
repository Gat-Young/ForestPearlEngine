#include "Renderer.h"
#include "DirectXMath.h"
#include <queue>
#include <assert.h>
#include <iostream>
#include <algorithm>

#include "../FPGameInstance.h"
#include "../FPAssetManager.h"
#include "../FPGameProjectSetting.h"

#include "FPRenderingCommon.h"
#include "../FPTextRenderList.h"
#include "../FPMeshRenderList.h"
#include "../FPCameraList.h"
#include "../FPGizmoRenderList.h"
#include "../FPViewPortClient.h"
#include "../FPLightRenderList.h"
#include "../FPMeshNormalRenderList.h"


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


Renderer::Renderer(RenderingDevice& Device) : Device(Device)
{
}

void Renderer::CreateCamList()
{
	//List에 값이 있다면 우선 비운
	CameraList.clear();

	//Camera List
	FPCameraList* CameraList = static_cast<FPCameraList*>(FPGameInstance::Get().GetCameraList());
	std::vector<CameraItem> CamList = CameraList->GetRenderList();

	for (CameraItem& CamItem : CamList)
	{
		//Cam이 활성 상태가 아니라면 넘어감
		if (!(*(CamItem.Active))) continue;

		std::vector<FPViewPort*> CamViewPorts;

		FPViewPortClient* ViewPortClient = static_cast<FPViewPortClient*>(FPGameInstance::Get().GetViewPortClient());

		CamViewPorts = ViewPortClient->GetViewPort(FPViewPortName::MainGameViewPort);

		//3분할 화면이라면 해당 ViewPort 정보로 반환
		if ((*(CamItem.TripleCam)))
		{
			CamViewPorts = ViewPortClient->GetViewPort(FPViewPortName::TripleWaySplitViewPort);
		}

		RenderingData::CameraItem RenderCamItem;
		RenderCamItem.Location = *(CamItem.Location);
		RenderCamItem.Rotation = *(CamItem.Rotation);
		RenderCamItem.Scale = *(CamItem.Scale);

		RenderCamItem.View = *(CamItem.View);
		RenderCamItem.Projection = *(CamItem.Projection);

		for (FPViewPort* ViewPort : CamViewPorts)
		{
			RenderingData::FPViewPort RenderingViewPortData;
			RenderingViewPortData.TopLeftX = ViewPort->TopLeftX;
			RenderingViewPortData.TopLeftY = ViewPort->TopLeftY;
			RenderingViewPortData.Width = ViewPort->Width;
			RenderingViewPortData.Height = ViewPort->Height;
			RenderingViewPortData.MinDepth = ViewPort->MinDepth;
			RenderingViewPortData.MaxDepth = ViewPort->MaxDepth;

			if(ViewPort->VertexConstBuffer != nullptr) RenderingViewPortData.VertexConstBuffer = *(ViewPort->VertexConstBuffer);
			if(ViewPort->PixelConstBuffer != nullptr) RenderingViewPortData.PixelConstBuffer = *(ViewPort->PixelConstBuffer);

			RenderCamItem.ViewPort.push_back(RenderingViewPortData);
		}
		this->CameraList.push_back(RenderCamItem);
	}



}

void Renderer::CreateMeshRenderList()
{
	//비어 있지 않다면 비우기
	MeshRenderList.clear();

	//Mesh Render List
	FPMeshRenderList* MeshRenderList = static_cast<FPMeshRenderList*>(FPGameInstance::Get().GetMeshRenderList());
	std::vector<RenderItem> RenderList = MeshRenderList->GetRenderList();

	for (RenderItem RenderItemData : RenderList)
	{
		//활성화된 객체가 아니라면 넘어감
		if (!(*(RenderItemData.Active)))
		{
			continue;
		}

		RenderingData::MeshRenderItem MeshRenderData;
		MeshRenderData.Priority = *(RenderItemData.Priority);
		MeshRenderData.MeshPath = *(RenderItemData.MeshPath);
		MeshRenderData.Location = *(RenderItemData.Location);
		MeshRenderData.Rotation = *(RenderItemData.Rotation);
		MeshRenderData.Scale = *(RenderItemData.Scale);
		MeshRenderData.VertexConstBuffer = *(RenderItemData.VertexConstBuffer);		//Material에 있는 모든 Vertex Constatnt Buffer를 넘겨 받음
		MeshRenderData.PixelConstBuffer = *(RenderItemData.PixelConstBuffer);		//Material에 있는 모든 Pixel Constatnt Buffer를 넘겨 받음

		if(RenderItemData.Resources != nullptr) MeshRenderData.Resources = *(RenderItemData.Resources);
		if(RenderItemData.Samplers != nullptr) MeshRenderData.Samplers = *(RenderItemData.Samplers);

		//모델링 행렬 계산
		FPMatrix WorldMatrix = ((MeshRenderData.Scale) * (MeshRenderData.Rotation) * (MeshRenderData.Location));
	

		//////////////////////////////////////////////////
		// 
		// Light RenderList 계산 (여기 변경 필요)
		//
		int MAX_LIGHT = 64;
		int LightRenderLoopSize = min(MAX_LIGHT, LightRenderList.size());
		int LightCount = 0;

		RenderingData::LightItem LightBuffer;
		LightBuffer.LightCnt = LightRenderLoopSize;

		//Light 정보
		for (LightCount = 0; LightCount < LightRenderLoopSize; ++LightCount)
		{
			RenderingData::LightDataItem LightData;

			LightData.LightType = LightRenderList[LightCount].LightType;

			LightData.LightPosition = LightRenderList[LightCount].Location.Matrix.r[3];

			FPVector4 LightDir = { LightRenderList[LightCount].Direction.x, LightRenderList[LightCount].Direction.y, LightRenderList[LightCount].Direction.z, 0.0f };
			LightData.Direction = -(LightDir.Normalize()); //Local로 변환
			
			LightData.Range = LightRenderList[LightCount].Range;
			LightData.Diffuse = LightRenderList[LightCount].Diffuse;
			LightData.Ambient = LightRenderList[LightCount].Ambient;
			LightData.Specular = LightRenderList[LightCount].Specular;
			LightData.Attenuation = LightRenderList[LightCount].Attenuation;
			LightData.LightColor = LightRenderList[LightCount].LightColor;


			LightBuffer.Light[LightCount] = LightData;

			
			/*
			std::cout << "\n========== LightDataItem ==========\n";

			RenderingData::LightDataItem& Data = LightData;
			std::cout << "LightCount : " << LightCount << "\n";
			std::cout << "LightType : " << Data.LightType << "\n";
			
			std::cout << "LightPosition : ("
				<< Data.LightPosition.x << ", "
				<< Data.LightPosition.y << ", "
				<< Data.LightPosition.z << ", "
				<< Data.LightPosition.w << ")\n";

			std::cout << "Direction : ("
				<< Data.Direction.x << ", "
				<< Data.Direction.y << ", "
				<< Data.Direction.z << ", "
				<< Data.Direction.w << ")\n";

			std::cout << "Range : " << Data.Range << "\n";

			std::cout << "Diffuse : ("
				<< Data.Diffuse.x << ", "
				<< Data.Diffuse.y << ", "
				<< Data.Diffuse.z << ", "
				<< Data.Diffuse.w << ")\n";

			std::cout << "Ambient : ("
				<< Data.Ambient.x << ", "
				<< Data.Ambient.y << ", "
				<< Data.Ambient.z << ", "
				<< Data.Ambient.w << ")\n";

			std::cout << "Specular : ("
				<< Data.Specular.x << ", "
				<< Data.Specular.y << ", "
				<< Data.Specular.z << ", "
				<< Data.Specular.w << ")\n";

			std::cout << "Attenuation : ("
				<< Data.Attenuation.x << ", "
				<< Data.Attenuation.y << ", "
				<< Data.Attenuation.z << ", "
				<< Data.Attenuation.w << ")\n";
				
			std::cout << "LightColor : ("
				<< Data.LightColor.x << ", "
				<< Data.LightColor.y << ", "
				<< Data.LightColor.z << ", "
				<< Data.LightColor.w << ")\n";

			std::cout << "===================================\n";
			*/
		}

		//VertexBuffer에 등록 2번은 Light
		FPConstantBufferRenderDataUtil::AddConstantBuffer(MeshRenderData.VertexConstBuffer, 2, LightBuffer);

		//Material 처리
		MeshRenderData.VertexShaderPath = *(RenderItemData.VertexShaderPath);
		MeshRenderData.PixelShaderPath = *(RenderItemData.PixelShaderPath);
		MeshRenderData.TwoSided = *(RenderItemData.TwoSided);
		this->MeshRenderList.push_back(MeshRenderData);
	}

	std::sort(this->MeshRenderList.begin(), this->MeshRenderList.end(), RenderingData::MeshRenderItemCompare{});

}

void Renderer::CreateGizmoRenderList()
{
	//비어 있지 않다면 비우기
	GizmoRenderList.clear();

	//Gizmo Render List
	FPGizmoRenderList* GizmoRenderList = static_cast<FPGizmoRenderList*>(FPGameInstance::Get().GetGizmoRenderList());
	std::vector<GizmoRenderItem> RenderList = GizmoRenderList->GetRenderList();

	for (GizmoRenderItem RenderItemData : RenderList)
	{
		//활성화된 객체가 아니라면 넘어감
		if (!(*(RenderItemData.Active)))
		{
			continue;
		}

		RenderingData::GizmoRenderItem GizmoRenderData;
		GizmoRenderData.Priority = *(RenderItemData.Priority);
		GizmoRenderData.MeshPath = *(RenderItemData.MeshPath);
		GizmoRenderData.Location = *(RenderItemData.Location);
		GizmoRenderData.Rotation = *(RenderItemData.Rotation);
		GizmoRenderData.Scale = *(RenderItemData.Scale);
		GizmoRenderData.VertexShaderPath = *(RenderItemData.VertexShaderPath);
		GizmoRenderData.PixelShaderPath = *(RenderItemData.PixelShaderPath);

		if (RenderItemData.VertexConstBuffer != nullptr) GizmoRenderData.VertexConstBuffer = *(RenderItemData.VertexConstBuffer);
		if (RenderItemData.PixelConstBuffer != nullptr) GizmoRenderData.PixelConstBuffer = *(RenderItemData.PixelConstBuffer);

		this->GizmoRenderList.push_back(GizmoRenderData);
	}

	std::sort(this->GizmoRenderList.begin(), this->GizmoRenderList.end(), RenderingData::GizmoRenderItemCompare{});

}

void Renderer::CreateUIRenderList()
{
	//비어 있지 않다면 비우기
	UIRenderList.clear();

	//Text Render List
	FPTextRenderList* TextRenderList = static_cast<FPTextRenderList*>(FPGameInstance::Get().GetTextRenderList());
	std::vector<UIContextItem> RenderList = TextRenderList->GetRenderList();

	//UI ViewPorts
	std::vector<FPViewPort*> CamViewPorts;
	FPViewPortClient* ViewPortClient = static_cast<FPViewPortClient*>(FPGameInstance::Get().GetViewPortClient());
	CamViewPorts = ViewPortClient->GetViewPort(FPViewPortName::UIViewPort);

	for (UIContextItem UIItem : RenderList)
	{
		//활성 상태가 아니라면 넘어감
		if (!(*(*(UIItem.active))))
		{
			continue;
		}

		RenderingData::UIContextItem UIRenderingData;
		UIRenderingData.Priority = *(UIItem.Priority);
		UIRenderingData.x = *(UIItem.x);
		UIRenderingData.y = *(UIItem.y);
		UIRenderingData.color = *(UIItem.color);
		UIRenderingData.msg = *(UIItem.msg);

		for (FPViewPort* ViewPort : CamViewPorts)
		{
			RenderingData::FPViewPort RenderingViewPortData;
			RenderingViewPortData.TopLeftX = ViewPort->TopLeftX;
			RenderingViewPortData.TopLeftY = ViewPort->TopLeftY;
			RenderingViewPortData.Width = ViewPort->Width;
			RenderingViewPortData.Height = ViewPort->Height;
			RenderingViewPortData.MinDepth = ViewPort->MinDepth;
			RenderingViewPortData.MaxDepth = ViewPort->MaxDepth;

			if (ViewPort->VertexConstBuffer != nullptr) RenderingViewPortData.VertexConstBuffer = *(ViewPort->VertexConstBuffer);
			if (ViewPort->PixelConstBuffer != nullptr) RenderingViewPortData.PixelConstBuffer = *(ViewPort->PixelConstBuffer);

			UIRenderingData.ViewPort.push_back(RenderingViewPortData);
		}

		UIRenderList.push_back(UIRenderingData);
	}

	std::sort(UIRenderList.begin(), UIRenderList.end(), RenderingData::UIRenderItemCompare{});
}

void Renderer::CreateLightRenderList()
{
	LightRenderList.clear();

	FPLightRenderList* LightRenderList = static_cast<FPLightRenderList*>(FPGameInstance::Get().GetLightRenderList());
	std::vector<FPLightRenderItem> RenderList = LightRenderList->GetRenderList();

	for (FPLightRenderItem LightItem : RenderList)
	{
		//활성 상태가 아니라면 넘어감
		if (!*(LightItem.Active))
		{
			continue;
		}

		RenderingData::LightRenderItem LightRenderingData;
		LightRenderingData.LightType = *(LightItem.LightType);
		LightRenderingData.Direction = *(LightItem.Direction);
		LightRenderingData.Range = *(LightItem.Range);
		LightRenderingData.Diffuse = *(LightItem.Diffuse);
		LightRenderingData.Ambient = *(LightItem.Ambient);
		LightRenderingData.Specular = *(LightItem.Specular);
		LightRenderingData.Attenuation = *(LightItem.Attenuation);
		LightRenderingData.LightColor = *(LightItem.LightColor);

		LightRenderingData.Location = *(LightItem.Location);
		LightRenderingData.Rotation = *(LightItem.Rotation);
		LightRenderingData.Scale = *(LightItem.Scale);

		LightRenderingData.VertexConstBuffer = *(LightItem.VertexConstBuffer);
		LightRenderingData.PixelConstBuffer = *(LightItem.PixelConstBuffer);

		this->LightRenderList.push_back(LightRenderingData);
	}


}

void Renderer::CreateNormalRenderList()
{
	//비어 있지 않다면 비우기
	NormalRenderList.clear();

	//Gizmo Render List
	FPMeshNormalRenderList* NormalRenderList = static_cast<FPMeshNormalRenderList*>(FPGameInstance::Get().GetNormalRenderList());
	std::vector<NormalRenderItem> RenderList = NormalRenderList->GetRenderList();

	for (NormalRenderItem RenderItemData : RenderList)
	{
		//활성화된 객체가 아니라면 넘어감
		if (!(*(RenderItemData.Active)))
		{
			continue;
		}

		RenderingData::NormalRenderItem NormalRenderData;
		NormalRenderData.MeshPath = *(RenderItemData.MeshPath);
		NormalRenderData.Location = *(RenderItemData.Location);
		NormalRenderData.Rotation = *(RenderItemData.Rotation);
		NormalRenderData.Scale = *(RenderItemData.Scale);
		NormalRenderData.NormalLength = *(RenderItemData.NormalLength);
		NormalRenderData.VertexShaderPath = *(RenderItemData.VertexShaderPath);
		NormalRenderData.PixelShaderPath = *(RenderItemData.PixelShaderPath);

		RenderingData::NormalDebugItem NormalItem;
		NormalItem.NormalLength = NormalRenderData.NormalLength;
		//모델링 행렬 계산
		FPMatrix WorldMatrix = ((NormalRenderData.Scale) * (NormalRenderData.Rotation) * (NormalRenderData.Location));

		const DirectX::XMMATRIX World = WorldMatrix.Matrix;
		//역행렬이 존재하는지 확인
		float Determinant = DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(World));

		if (!std::isfinite(Determinant) || std::abs(Determinant) < 1e-8f)
		{
			// 이 Mesh의 Normal Debug 렌더링 생략
			continue;
		}

		NormalItem.NormalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, World));

		//VB 설정
		FPConstantBufferRenderDataUtil::AddConstantBuffer(NormalRenderData.VertexConstBuffer, 2, NormalItem);

		this->NormalRenderList.push_back(NormalRenderData);

		//뒷면 노말
		NormalItem.NormalSign = -1.0f;
		FPConstantBufferRenderDataUtil::UpdateConstantBuffer(NormalRenderData.VertexConstBuffer, 0, NormalItem);
		this->NormalRenderList.push_back(NormalRenderData);
	}
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

	//9232B의 ConstBuffer 생성
	Device.CreateVertexShaderConstBuffer(9232);
	Device.CreatePixelShaderConstBuffer(9232);

	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}
	return hr;
}

void Renderer::MeshRenderPass()
{
	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	//Depth Dtencill 상태 설정
	Device.OMSetDepthStencilState(ZEnable);

	Device.OMSetBlendState(BS_ADD);


	//기하 위상 구조 설정
	Device.IASetPrimitiveTopology(Topology::TRIANGLELIST);

	//상수 버퍼 설정
	//Vertex Shader
	//MeshRenderPass에서 0번은 WVP, 1번은 ViewPort용 슬롯, 2번은 Light용 슬롯 임
	//PixelShader
	//MeshRenderPass에서 0번은 ViewPort용 슬롯


	RenderingData::MVPConstBuffer MVPCB;

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	//std::cout << CameraList.size() << " : " << MeshRenderList.size() << "\n";

	for (RenderingData::CameraItem& CamItem : CameraList)
	{
		for(const RenderingData::MeshRenderItem& RenderItem : MeshRenderList)
		{
			/*
			std::cout << RenderItem.MeshPath << "\n";
			std::cout << RenderItem.TextureList.size() << "\n";
			std::cout << RenderItem.VertexShaderPath << "\n";
			std::cout << RenderItem.PixelShaderPath << "\n";
			*/

			//렌더링 모드 전환
			Device.UpdateRSSetState(bFill, !(RenderItem.TwoSided));

			std::tuple<void*, void*, void*> VertexShaderData = AssetManager->GetVertexShader(RenderItem.VertexShaderPath);
			std::pair<void*, void*> PixelShaderData = AssetManager->GetPixelShader(RenderItem.PixelShaderPath);

			//입력 레이아웃 설정
			Device.IASetInputLayout(std::get<2>(VertexShaderData));

			//Shader 설정
			Device.VSSetShader(std::get<0>(VertexShaderData));
			Device.PSSetShader(std::get<0>(PixelShaderData));

			//Shader ConstBuffer 갱신
			
			//vertex
			for (FPConstantBufferInfo ConstantBufferInfo : RenderItem.VertexConstBuffer.ConstantBuffers)
			{
				unsigned int Slot = ConstantBufferInfo.Slot;
				size_t Offset = ConstantBufferInfo.Offset;
				size_t Size = ConstantBufferInfo.Size;

				Device.UpdateVertexShaderSubresource(Slot, RenderItem.VertexConstBuffer.Buffer.data(), Offset, Size);
			}

			//Pixel
			for (FPConstantBufferInfo ConstantBufferInfo : RenderItem.PixelConstBuffer.ConstantBuffers)
			{
				unsigned int Slot = ConstantBufferInfo.Slot;
				size_t Offset = ConstantBufferInfo.Offset;
				size_t Size = ConstantBufferInfo.Size;

				Device.UpdatePixelShaderSubresource(Slot, RenderItem.PixelConstBuffer.Buffer.data(), Offset, Size);
			}

			//Texture 셋팅
			const std::vector<FPReflectionResourceInfo>& Resources = RenderItem.Resources;
			
			for (const FPReflectionResourceInfo& TextureItem : Resources)
			{
				void* TextureData = nullptr;
				//Texture 가져옴
				if (TextureItem.TextureType == Texture2D) { TextureData = AssetManager->GetTextureData(TextureItem.AssetName).TextureResource; }
				if (TextureItem.TextureType == Texture2DArray) { TextureData = AssetManager->GetTextureArrayData(TextureItem.AssetName).TextureArraySRV; }

				Device.PSSetShaderResources(TextureItem.Slot, 1, TextureData);
			}
			//Texture 셋팅 End.

			//Sampler 셋팅
			const std::vector<FPReflectionSamplerInfo>& Samplers = RenderItem.Samplers;

			for (const FPReflectionSamplerInfo& SamplerItem : Samplers)
			{
				Device.PSSetSamplers(SamplerItem.Slot, 1, SamplerItem.SamplerType);
			}

			//Sampler 셋팅 End.
			FPVertexBufferData PositionVB = AssetManager->GetPositionVertexBuffer(RenderItem.MeshPath);
			FPVertexBufferData NormalVB = AssetManager->GetNormalVertexBuffer(RenderItem.MeshPath);
			FPVertexBufferData ColorVB = AssetManager->GetColorVertexBuffer(RenderItem.MeshPath);
			FPVertexBufferData UVVB = AssetManager->GetUVVertexBuffer(RenderItem.MeshPath);
			FPIndexBufferData IndexB = AssetManager->GetIndexBuffer(RenderItem.MeshPath);
			/*
			std::cout << "<VB Size>" << "\n";
			std::cout << PositionVB.VB_Cnt << "\n";
			std::cout << NormalVB.VB_Cnt << "\n";
			std::cout << ColorVB.VB_Cnt << "\n";
			std::cout << UVVB.VB_Cnt << "\n";
			std::cout << IndexB.Index_Cnt << "\n";
			*/

			for (RenderingData::FPViewPort& CamViewPort : CamItem.ViewPort)
			{

				Device.SetViewPort(CamViewPort.TopLeftX, CamViewPort.TopLeftY,
					CamViewPort.Width, CamViewPort.Height,
					CamViewPort.MinDepth, CamViewPort.MaxDepth);

				//Mesh에서 가져오는 정보
				//HLSL은 열벡터 기준이므로 HLSL에서는 연산을 반대로 할 것
				MVPCB.WorldMatrix = ((RenderItem.Scale) * (RenderItem.Rotation) * (RenderItem.Location)).Matrix;
				MVPCB.InverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WorldMatrix);
				MVPCB.TranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.WorldMatrix);
				MVPCB.InverseTranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWorldMatrix);

				MVPCB.ViewMatrix = (CamItem.View).Matrix;
				MVPCB.InverseViewMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.ViewMatrix);
				MVPCB.TranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.ViewMatrix);
				MVPCB.InverseTranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseViewMatrix);

				MVPCB.ProjMatrix = (CamItem.Projection).Matrix;
				MVPCB.InverseProjMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.ProjMatrix);
				MVPCB.TranseposeProjMatrix = DirectX::XMMatrixTranspose(MVPCB.ProjMatrix);
				MVPCB.InverseTranseposeProjMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseProjMatrix);

				MVPCB.WVMatrix = MVPCB.WorldMatrix * MVPCB.ViewMatrix;
				MVPCB.InverseWVMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WVMatrix);
				MVPCB.TranseposeWVMatrix = DirectX::XMMatrixTranspose(MVPCB.WVMatrix);
				MVPCB.InverseTranseposeWVMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWVMatrix);

				MVPCB.WVPMatrix = MVPCB.WorldMatrix * MVPCB.ViewMatrix * MVPCB.ProjMatrix;
				MVPCB.InverseWVPMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WVPMatrix);
				MVPCB.TranseposeWVPMatrix = DirectX::XMMatrixTranspose(MVPCB.WVPMatrix);
				MVPCB.InverseTranseposeWVPMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWVPMatrix);

				//Object 상수 버퍼 갱신
				Device.UpdateVertexShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);


				//ViewPort Shader ConstBuffer가 있다면 갱신
				//vertex
				for (FPConstantBufferInfo ConstantBufferInfo : CamViewPort.VertexConstBuffer.ConstantBuffers)
				{
					unsigned int Slot = ConstantBufferInfo.Slot;
					size_t Offset = ConstantBufferInfo.Offset;
					size_t Size = ConstantBufferInfo.Size;

					Device.UpdateVertexShaderSubresource(Slot, CamViewPort.VertexConstBuffer.Buffer.data(), Offset, Size);
				}

				//Pixel
				for (FPConstantBufferInfo ConstantBufferInfo : CamViewPort.PixelConstBuffer.ConstantBuffers)
				{
					unsigned int Slot = ConstantBufferInfo.Slot;
					size_t Offset = ConstantBufferInfo.Offset;
					size_t Size = ConstantBufferInfo.Size;

					Device.UpdatePixelShaderSubresource(Slot, CamViewPort.PixelConstBuffer.Buffer.data(), Offset, Size);
				}
				/////////////////////


				Device.IASetVertexBuffers(0, 1, PositionVB.VertexBuffer, &(PositionVB.Stride), &(PositionVB.Offset));
				Device.IASetVertexBuffers(1, 1, NormalVB.VertexBuffer, &(NormalVB.Stride), &(NormalVB.Offset));
				Device.IASetVertexBuffers(2, 1, ColorVB.VertexBuffer, &(ColorVB.Stride), &(ColorVB.Offset));
				Device.IASetVertexBuffers(3, 1, UVVB.VertexBuffer, &(UVVB.Stride), &(UVVB.Offset));
				Device.IASetIndexBuffers(IndexB.IndexBuffer, IndexB.Offset);

				Device.DrawIndexed(IndexB.Index_Cnt, 0, 0);																//Draw Call
			}
		}
	}

	Device.OMSetBlendState(BS_NULL_);
}


void Renderer::GizmoRenderPass()
{
	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	//Depth Dtencill 상태 설정
	Device.OMSetDepthStencilState(ZEnable);

	//렌더링 모드 전환
	Device.UpdateRSSetState(bFill, bCull);

	//기하 위상 구조 설정
	Device.IASetPrimitiveTopology(Topology::LINELIST);

	//상수 버퍼 설정
	//Vertex Shader
	//MeshRenderPass에서 0번은 WVP, 1번은 ViewPort용 슬롯 임
	//Slot is the absolute register number (b#) in all passes: b0 Object, b1 ViewPort, b2 Light / NormalDebug

	//PixelShader
	//MeshRenderPass에서 0번은 ViewPort용 슬롯


	RenderingData::MVPConstBuffer MVPCB;

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	for (RenderingData::CameraItem& CamItem : CameraList)
	{
		for (const RenderingData::GizmoRenderItem& RenderItem : GizmoRenderList)
		{
			std::tuple<void*, void*, void*> VertexShaderData = AssetManager->GetVertexShader(RenderItem.VertexShaderPath);
			std::pair<void*, void*> PixelShaderData = AssetManager->GetPixelShader(RenderItem.PixelShaderPath);

			//입력 레이아웃 설정
			Device.IASetInputLayout(std::get<2>(VertexShaderData));

			//Shader 설정
			Device.VSSetShader(std::get<0>(VertexShaderData));
			Device.PSSetShader(std::get<0>(PixelShaderData));

			//Shader ConstBuffer 갱신

			//vertex
			for (FPConstantBufferInfo ConstantBufferInfo : RenderItem.VertexConstBuffer.ConstantBuffers)
			{
				unsigned int Slot = ConstantBufferInfo.Slot;
				size_t Offset = ConstantBufferInfo.Offset;
				size_t Size = ConstantBufferInfo.Size;

				Device.UpdateVertexShaderSubresource(Slot, RenderItem.VertexConstBuffer.Buffer.data(), Offset, Size);
			}

			//Pixel
			for (FPConstantBufferInfo ConstantBufferInfo : RenderItem.PixelConstBuffer.ConstantBuffers)
			{
				unsigned int Slot = ConstantBufferInfo.Slot;
				size_t Offset = ConstantBufferInfo.Offset;
				size_t Size = ConstantBufferInfo.Size;

				Device.UpdatePixelShaderSubresource(Slot, RenderItem.PixelConstBuffer.Buffer.data(), Offset, Size);
			}

			FPVertexBufferData PositionVB = AssetManager->GetPositionVertexBuffer(RenderItem.MeshPath);
			FPVertexBufferData ColorVB = AssetManager->GetColorVertexBuffer(RenderItem.MeshPath);

			for (RenderingData::FPViewPort& CamViewPort : CamItem.ViewPort)
			{

				Device.SetViewPort(CamViewPort.TopLeftX, CamViewPort.TopLeftY,
					CamViewPort.Width, CamViewPort.Height,
					CamViewPort.MinDepth, CamViewPort.MaxDepth);

				//HLSL은 열벡터 기준이므로 HLSL에서는 연산을 반대로 할 것
				MVPCB.WorldMatrix = ((RenderItem.Scale) * (RenderItem.Rotation) * (RenderItem.Location)).Matrix;
				MVPCB.InverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WorldMatrix);
				MVPCB.TranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.WorldMatrix);
				MVPCB.InverseTranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWorldMatrix);

				MVPCB.ViewMatrix = (CamItem.View).Matrix;
				MVPCB.InverseViewMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.ViewMatrix);
				MVPCB.TranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.ViewMatrix);
				MVPCB.InverseTranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseViewMatrix);

				MVPCB.ProjMatrix = (CamItem.Projection).Matrix;
				MVPCB.InverseProjMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.ProjMatrix);
				MVPCB.TranseposeProjMatrix = DirectX::XMMatrixTranspose(MVPCB.ProjMatrix);
				MVPCB.InverseTranseposeProjMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseProjMatrix);

				MVPCB.WVMatrix = MVPCB.WorldMatrix * MVPCB.ViewMatrix;
				MVPCB.InverseWVMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WVMatrix);
				MVPCB.TranseposeWVMatrix = DirectX::XMMatrixTranspose(MVPCB.WVMatrix);
				MVPCB.InverseTranseposeWVMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWVMatrix);

				MVPCB.WVPMatrix = MVPCB.WorldMatrix * MVPCB.ViewMatrix * MVPCB.ProjMatrix;
				MVPCB.InverseWVPMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WVPMatrix);
				MVPCB.TranseposeWVPMatrix = DirectX::XMMatrixTranspose(MVPCB.WVPMatrix);
				MVPCB.InverseTranseposeWVPMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWVPMatrix);

				//Object 상수 버퍼 갱신
				Device.UpdateVertexShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);

				//ViewPort Shader ConstBuffer가 있다면 갱신
				//vertex
				for (FPConstantBufferInfo ConstantBufferInfo : CamViewPort.VertexConstBuffer.ConstantBuffers)
				{
					unsigned int Slot = ConstantBufferInfo.Slot;
					size_t Offset = ConstantBufferInfo.Offset;
					size_t Size = ConstantBufferInfo.Size;

					Device.UpdateVertexShaderSubresource(Slot, CamViewPort.VertexConstBuffer.Buffer.data(), Offset, Size);
				}

				//Pixel
				for (FPConstantBufferInfo ConstantBufferInfo : CamViewPort.PixelConstBuffer.ConstantBuffers)
				{
					unsigned int Slot = ConstantBufferInfo.Slot;
					size_t Offset = ConstantBufferInfo.Offset;
					size_t Size = ConstantBufferInfo.Size;

					Device.UpdatePixelShaderSubresource(Slot, CamViewPort.PixelConstBuffer.Buffer.data(), Offset, Size);
				}


				Device.IASetVertexBuffers(0, 1, PositionVB.VertexBuffer, &(PositionVB.Stride), &(PositionVB.Offset));
				Device.IASetVertexBuffers(2, 1, ColorVB.VertexBuffer, &(ColorVB.Stride), &(ColorVB.Offset));

				Device.Draw(PositionVB.VB_Cnt, 0);																		//Draw Call
			}
		}
	}
}

void Renderer::NormalRenderPass()
{
	if (!bNormal) return;

	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	//Depth Dtencill 상태 설정
	Device.OMSetDepthStencilState(ZEnable);

	//렌더링 모드 전환
	Device.UpdateRSSetState(bFill, bCull);

	//기하 위상 구조 설정
	Device.IASetPrimitiveTopology(Topology::LINELIST);

	//상수 버퍼 설정
	//Vertex Shader
	//MeshRenderPass에서 0번은 WVP, 1번은 ViewPort용 슬롯 임
	//Slot is the absolute register number (b#) in all passes: b0 Object, b1 ViewPort, b2 Light / NormalDebug

	//PixelShader
	//MeshRenderPass에서 0번은 ViewPort용 슬롯


	RenderingData::MVPConstBuffer MVPCB;

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	for (RenderingData::CameraItem& CamItem : CameraList)
	{
		for (const RenderingData::NormalRenderItem& RenderItem : NormalRenderList)
		{
			std::tuple<void*, void*, void*> VertexShaderData = AssetManager->GetVertexShader(RenderItem.VertexShaderPath);
			std::pair<void*, void*> PixelShaderData = AssetManager->GetPixelShader(RenderItem.PixelShaderPath);

			//입력 레이아웃 설정
			Device.IASetInputLayout(std::get<2>(VertexShaderData));

			//Shader 설정
			Device.VSSetShader(std::get<0>(VertexShaderData));
			Device.PSSetShader(std::get<0>(PixelShaderData));

			//Shader ConstBuffer 갱신

			//vertex
			for (FPConstantBufferInfo ConstantBufferInfo : RenderItem.VertexConstBuffer.ConstantBuffers)
			{
				unsigned int Slot = ConstantBufferInfo.Slot;
				size_t Offset = ConstantBufferInfo.Offset;
				size_t Size = ConstantBufferInfo.Size;

				Device.UpdateVertexShaderSubresource(Slot, RenderItem.VertexConstBuffer.Buffer.data(), Offset, Size);
			}

			//Pixel
			for (FPConstantBufferInfo ConstantBufferInfo : RenderItem.PixelConstBuffer.ConstantBuffers)
			{
				unsigned int Slot = ConstantBufferInfo.Slot;
				size_t Offset = ConstantBufferInfo.Offset;
				size_t Size = ConstantBufferInfo.Size;

				Device.UpdatePixelShaderSubresource(Slot, RenderItem.PixelConstBuffer.Buffer.data(), Offset, Size);
			}

			FPVertexBufferData PositionVB = AssetManager->GetPositionVertexBuffer(RenderItem.MeshPath);
			FPVertexBufferData NormalVB = AssetManager->GetNormalVertexBuffer(RenderItem.MeshPath);
			FPIndexBufferData IndexB = AssetManager->GetIndexBuffer(RenderItem.MeshPath);

			for (RenderingData::FPViewPort& CamViewPort : CamItem.ViewPort)
			{

				Device.SetViewPort(CamViewPort.TopLeftX, CamViewPort.TopLeftY,
					CamViewPort.Width, CamViewPort.Height,
					CamViewPort.MinDepth, CamViewPort.MaxDepth);

				//HLSL은 열벡터 기준이므로 HLSL에서는 연산을 반대로 할 것
				MVPCB.WorldMatrix = ((RenderItem.Scale) * (RenderItem.Rotation) * (RenderItem.Location)).Matrix;
				MVPCB.InverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WorldMatrix);
				MVPCB.TranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.WorldMatrix);
				MVPCB.InverseTranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWorldMatrix);

				MVPCB.ViewMatrix = (CamItem.View).Matrix;
				MVPCB.InverseViewMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.ViewMatrix);
				MVPCB.TranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.ViewMatrix);
				MVPCB.InverseTranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseViewMatrix);

				MVPCB.ProjMatrix = (CamItem.Projection).Matrix;
				MVPCB.InverseProjMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.ProjMatrix);
				MVPCB.TranseposeProjMatrix = DirectX::XMMatrixTranspose(MVPCB.ProjMatrix);
				MVPCB.InverseTranseposeProjMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseProjMatrix);

				MVPCB.WVMatrix = MVPCB.WorldMatrix * MVPCB.ViewMatrix;
				MVPCB.InverseWVMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WVMatrix);
				MVPCB.TranseposeWVMatrix = DirectX::XMMatrixTranspose(MVPCB.WVMatrix);
				MVPCB.InverseTranseposeWVMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWVMatrix);

				MVPCB.WVPMatrix = MVPCB.WorldMatrix * MVPCB.ViewMatrix * MVPCB.ProjMatrix;
				MVPCB.InverseWVPMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WVPMatrix);
				MVPCB.TranseposeWVPMatrix = DirectX::XMMatrixTranspose(MVPCB.WVPMatrix);
				MVPCB.InverseTranseposeWVPMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWVPMatrix);

				//Object 상수 버퍼 갱신
				Device.UpdateVertexShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);

				//ViewPort Shader ConstBuffer가 있다면 갱신
				//vertex
				for (FPConstantBufferInfo ConstantBufferInfo : CamViewPort.VertexConstBuffer.ConstantBuffers)
				{
					unsigned int Slot = ConstantBufferInfo.Slot;
					size_t Offset = ConstantBufferInfo.Offset;
					size_t Size = ConstantBufferInfo.Size;

					Device.UpdateVertexShaderSubresource(Slot, CamViewPort.VertexConstBuffer.Buffer.data(), Offset, Size);
				}

				//Pixel
				for (FPConstantBufferInfo ConstantBufferInfo : CamViewPort.PixelConstBuffer.ConstantBuffers)
				{
					unsigned int Slot = ConstantBufferInfo.Slot;
					size_t Offset = ConstantBufferInfo.Offset;
					size_t Size = ConstantBufferInfo.Size;

					Device.UpdatePixelShaderSubresource(Slot, CamViewPort.PixelConstBuffer.Buffer.data(), Offset, Size);
				}

				Device.IASetVertexBuffers(0, 1, PositionVB.VertexBuffer, &(PositionVB.Stride), &(PositionVB.Offset));
				Device.IASetVertexBuffers(1, 1, NormalVB.VertexBuffer, &(NormalVB.Stride), &(NormalVB.Offset));
				Device.IASetIndexBuffers(IndexB.IndexBuffer, IndexB.Offset);

				Device.DrawInstanced(2, PositionVB.VB_Cnt, 0, 0);																		//Draw Call
			}
		}
	}
}

void Renderer::UIRenderPass()
{
	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	for (const RenderingData::UIContextItem& UIData : UIRenderList)
	{
		FontBatch->Begin();
		for (RenderingData::FPViewPort CamViewPort : UIData.ViewPort)
		{
			Device.SetViewPort(CamViewPort.TopLeftX, CamViewPort.TopLeftY,
				CamViewPort.Width, CamViewPort.Height,
				CamViewPort.MinDepth, CamViewPort.MaxDepth);

			XMFLOAT4 Color = { (UIData.color.x), (UIData.color.y), (UIData.color.z), (UIData.color.w) };
			XMFLOAT2 Position = { (float)((UIData.x)), (float)((UIData.y)) };
			Font->DrawString(FontBatch, UIData.msg.c_str(), Position, XMLoadFloat4(&Color));
		}
		FontBatch->End();
	}

}


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