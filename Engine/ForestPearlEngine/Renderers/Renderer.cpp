#include "Renderer.h"
#include "DirectXMath.h"
#include <queue>
#include <assert.h>
#include <iostream>
#include <algorithm>

#include "../FPGameInstance.h"
#include "../FPAssetManager.h"
#include "../FPMaterialInterface.h"
#include "../FPGameProjectSetting.h"

#include "FPRenderingCommon.h"
#include "../FPTextRenderList.h"
#include "../FPMeshRenderList.h"
#include "../FPCameraList.h"
#include "../FPGizmoRenderList.h"
#include "../FPLightRenderList.h"
#include "../FPSkyLightRenderList.h"
#include "../FPMeshNormalRenderList.h"
#include "../FPSceneCaptureList.h"
#include "../FPPlanarReflectionList.h"

//맘에 안들기는 하는 데 우선..
#include "../Object/Actor.h"


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


void Renderer::SetRasterizerState(FPAssetManager* AssetManager, bool bFillSolid, bool bCullBack, bool bFlipCull)
{
	std::string Name;
	if (bFillSolid) Name = bCullBack ? "RS_CULLBACK" : "RS_SOLID";
	else Name = bCullBack ? "RS_WIRECULLBACK" : "RS_WIREFRM";

	//반사 등으로 핸드니스가 뒤집히는 경우 CullMode는 그대로 두고 FrontCounterClockwise만 뒤집은 전용 State 사용
	if (bFlipCull) Name += "_FCCW";

	FPRasterizerStateData& RasterizerStateData = AssetManager->GetRasterizerStateData(Name);
	Device.RSSetState(RasterizerStateData.RasterizerStateData);
}

void Renderer::Quad2DRenderPass(std::string& InRenderTarget)
{
	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	//화면 해상도 정보를 가져옴
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());

	float DisplayWidth = GameProjectSetting->GetDisplayWidth();
	float DisplayHeight = GameProjectSetting->GetDeisplayHeight();


	//////////////////////////////////////////////////////////////////////////
	// 
	// 필요한 행렬 연산
	// 
	//Quad용 모델링 행렬
	RenderingData::MVPConstBuffer MVPCB;
	
	//단위 행렬 사용
	MakeQuad2DMVP(MVPCB, DisplayWidth, DisplayHeight);
	//
	//
	//
	///////////////////////////////////////////////////////////////////////////

	//렌더링 관련 설정
	
	// BackBuffer Setting
	//BackBuffer RenderTarget / DepthStencil Default Set
	FPDepthStencilData& BackBufferDepthStencilData = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager())->GetDepthStencilData("BackBufferDepthStencil");
	Device.OMSetBackBufferRenderTargets(1, BackBufferDepthStencilData.DepthStencilView);

	//컬링 OFF
	SetRasterizerState(AssetManager, bFill, false);

	//Blend 설정 우선 OFF
	Device.OMSetBlendState(static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager())->GetBlendStateData("BS_DEFAULT").BlendStateData);

	//Topology 설정
	Device.IASetPrimitiveTopology(Topology::TRIANGLESTRIP);

	//깊이버퍼 쓰기는 우선 OFF
	FPDepthStencilStateData& DepthStencilStateData = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager())->GetDepthStencilStateData("DS_DEPTH_OFF");
	Device.OMSetDepthStencilState(DepthStencilStateData.DepthStencilData);

	//ViewPort설정 우선 전체 화면
	Device.SetViewPort(0, 0, (float)DisplayWidth, (float)DisplayHeight, 0, 1);

	//셰이더 / InputLayout Set
	std::string VertexShaderPath = "DefaultQuad2DVertexShader";
	std::string PixelShaderPath = "DefaultQuad2DPixelShader";
	std::tuple<void*, void*, void*> VertexShaderData = AssetManager->GetVertexShader(VertexShaderPath);
	std::pair<void*, void*> PixelShaderData = AssetManager->GetPixelShader(PixelShaderPath);

	//입력 레이아웃 설정
	Device.IASetInputLayout(std::get<2>(VertexShaderData));

	//Shader 설정
	Device.VSSetShader(std::get<0>(VertexShaderData));
	Device.PSSetShader(std::get<0>(PixelShaderData));

	//Object 상수 버퍼 갱신
	Device.UpdateVertexShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);
	RenderingData::ClipPlanarReflectionConstBuffer ClipCB;
	Device.UpdateVertexShaderSubresource(13, reinterpret_cast<const uint8_t*>(&ClipCB), 0, sizeof(RenderingData::ClipPlanarReflectionConstBuffer));
	Device.UpdatePixelShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);

	//Texture 세팅
	void* TextureData = nullptr;

	//Texture 가져옴 (우선, RenderTarget에 있는 데이터만 가져옴)
	TextureData = AssetManager->GetRenderTargetData(InRenderTarget).ShaderResourceView;
	
	//Texture 설정
	Device.PSSetShaderResources(0, 1, TextureData);

	//텍스처 샘플러는 Clamp를 씀
	Device.PSSetSamplers(0, 1, FPSamplerType::Filter_Tri_Linear_Sampler_CLAMP);

	//Quad Mesh Setting
	FPVertexBufferData PositionVB = AssetManager->GetPositionVertexBuffer("Quad2D_Mesh");
	FPVertexBufferData NormalVB = AssetManager->GetNormalVertexBuffer("Quad2D_Mesh");
	FPVertexBufferData ColorVB = AssetManager->GetColorVertexBuffer("Quad2D_Mesh");
	FPVertexBufferData UVVB = AssetManager->GetUVVertexBuffer("Quad2D_Mesh");
	FPIndexBufferData IndexB = AssetManager->GetIndexBuffer("Quad2D_Mesh");
	FPVertexBufferData TangentVB = AssetManager->GetTangentVertexBuffer("Quad2D_Mesh");

	Device.IASetVertexBuffers(0, 1, PositionVB.VertexBuffer, &(PositionVB.Stride), &(PositionVB.Offset));
	Device.IASetVertexBuffers(1, 1, NormalVB.VertexBuffer, &(NormalVB.Stride), &(NormalVB.Offset));
	Device.IASetVertexBuffers(2, 1, ColorVB.VertexBuffer, &(ColorVB.Stride), &(ColorVB.Offset));
	Device.IASetVertexBuffers(3, 1, UVVB.VertexBuffer, &(UVVB.Stride), &(UVVB.Offset));
	Device.IASetVertexBuffers(4, 1, TangentVB.VertexBuffer, &(TangentVB.Stride), &(TangentVB.Offset));
	Device.IASetIndexBuffers(IndexB.IndexBuffer, IndexB.Offset);

	Device.DrawIndexed(IndexB.Index_Cnt, 0, 0);
}

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

		RenderingData::CameraItem RenderCamItem;
		RenderCamItem.Location = *(CamItem.Location);
		RenderCamItem.Rotation = *(CamItem.Rotation);
		RenderCamItem.Scale = *(CamItem.Scale);

		RenderCamItem.View = *(CamItem.View);
		RenderCamItem.Projection = *(CamItem.Projection);

		this->CameraList.push_back(RenderCamItem);
	}



}

void Renderer::CreateMeshRenderList()
{
	//비어 있지 않다면 비우기
	OpaqueRenderList.clear();
	MaskedRenderList.clear();
	TranslucentRenderList.clear();
	SkyBoxRenderList.clear();
	PlanarReflectionOpaqueRenderList.clear();
	PlanarReflectionMaskedRenderList.clear();
	PlanarReflectionmTranslucentRenderList.clear();

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
		MeshRenderData.ActorName = static_cast<FPActor*>(RenderItemData.OwnActor)->GetName();		//살짝 맘에 들지는 않네...
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
		//FPConstantBufferRenderDataUtil::AddConstantBuffer(RenderItemData.VertexConstBuffer, 2, LightBuffer);

		//PixelBuffer에 등록 2번은 Light
		FPConstantBufferRenderDataUtil::AddConstantBuffer(MeshRenderData.PixelConstBuffer, 2, LightBuffer);

		//SkyLight 처리 (PixelBuffer의 슬롯 1번은 SkyLight)
		RenderingData::FPSkyLightDataItem SkyLightDataItem{};
		if (!FPSkyLightList.empty())
		{
			SkyLightDataItem = FPSkyLightList[0].SkyLightDataItem;
		}

		FPConstantBufferRenderDataUtil::AddConstantBuffer(MeshRenderData.PixelConstBuffer, 1, SkyLightDataItem);

		//Material 처리
		MeshRenderData.VertexShaderPath = *(RenderItemData.VertexShaderPath);
		MeshRenderData.PixelShaderPath = *(RenderItemData.PixelShaderPath);
		MeshRenderData.TwoSided = *(RenderItemData.TwoSided);
		MeshRenderData.BlendState = *(RenderItemData.BlendState);
		MeshRenderData.Detail = *(RenderItemData.Detail);
		MeshRenderData.bIsSky = *(RenderItemData.bIsSky);
		MeshRenderData.bUsePlanarForwardReflections = *(RenderItemData.bUsePlanarForwardReflections);
		MeshRenderData.StencilRefValue = *(RenderItemData.StencilRefValue);
		MeshRenderData.bCustomDepthStencil = *(RenderItemData.bCustomDepthStencil);

		//Material Detail에 따라 각각의 RenderPass에 넣음
		if (MeshRenderData.bIsSky)
		{
			this->SkyBoxRenderList.push_back(MeshRenderData);
		}
		else if (MeshRenderData.bUsePlanarForwardReflections)
		{
			switch (MeshRenderData.Detail)
			{
				case FPMaterialDetail::Masked:
					this->PlanarReflectionMaskedRenderList.push_back(MeshRenderData);
					break;
				case FPMaterialDetail::Translucent:
					this->PlanarReflectionmTranslucentRenderList.push_back(MeshRenderData);
					break;
				default:
					this->PlanarReflectionOpaqueRenderList.push_back(MeshRenderData);
					break;
			}
		}
		else
		{
			switch (MeshRenderData.Detail)
			{
				case FPMaterialDetail::Masked:
					this->MaskedRenderList.push_back(MeshRenderData);
					break;
				case FPMaterialDetail::Translucent:
					this->TranslucentRenderList.push_back(MeshRenderData);
					break;
				default:
					this->OpaqueRenderList.push_back(MeshRenderData);
					break;
			}
		}
	}

	std::sort(this->OpaqueRenderList.begin(), this->OpaqueRenderList.end(), RenderingData::MeshRenderItemCompare{});
	std::sort(this->MaskedRenderList.begin(), this->MaskedRenderList.end(), RenderingData::MeshRenderItemCompare{});
	std::sort(this->TranslucentRenderList.begin(), this->TranslucentRenderList.end(), RenderingData::MeshRenderItemCompare{});
	std::sort(this->SkyBoxRenderList.begin(), this->SkyBoxRenderList.end(), RenderingData::MeshRenderItemCompare{});
	std::sort(this->PlanarReflectionOpaqueRenderList.begin(), this->PlanarReflectionOpaqueRenderList.end(), RenderingData::MeshRenderItemCompare{});
	std::sort(this->PlanarReflectionMaskedRenderList.begin(), this->PlanarReflectionMaskedRenderList.end(), RenderingData::MeshRenderItemCompare{});
	std::sort(this->PlanarReflectionmTranslucentRenderList.begin(), this->PlanarReflectionmTranslucentRenderList.end(), RenderingData::MeshRenderItemCompare{});

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
		this->LightRenderList.push_back(LightRenderingData);
	}


}

void Renderer::CreateSkyLightRenderList()
{
	FPSkyLightList.clear();

	FPSkyLightRenderList* SkyLightRenderList = static_cast<FPSkyLightRenderList*>(FPGameInstance::Get().GetSkyLightRenderList());
	std::vector<FPSkyLightRenderItem> RenderList = SkyLightRenderList->GetRenderList();

	for (FPSkyLightRenderItem SkyLightItem : RenderList)
	{
		//활성 상태가 아니라면 넘어감
		if (!*(SkyLightItem.Active))
		{
			continue;
		}

		RenderingData::FPSkyLightRenderItem SkyLightRenderingData;
		SkyLightRenderingData.SkyLightDataItem.LightColor = *(SkyLightItem.LightColor);
		SkyLightRenderingData.SkyLightDataItem.LightIntensity = *(SkyLightItem.LightIntensity);
		SkyLightRenderingData.CubeMap = *(SkyLightItem.CubeMap);

		FPSkyLightList.push_back(SkyLightRenderingData);
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

void Renderer::CreateSceneCaptureList()
{
	//List에 값이 있다면 우선 비운
	SceneCaptureList.clear();

	//Camera List
	FPSceneCaptureList* SceneCaptureList = static_cast<FPSceneCaptureList*>(FPGameInstance::Get().GetSceneCaptureList());
	std::vector<SceneCaptureItem> SceneCapList = SceneCaptureList->GetRenderList();

	for (SceneCaptureItem& SceneCaptureItem : SceneCapList)
	{
		//CaptureComponent가 활성 상태가 아니라면 넘어감
		if (!(*(SceneCaptureItem.Active))) continue;

		bool CaptureEveryFrame = *(SceneCaptureItem.CaptureEveryFrame);
		bool CaptureThisFrame = *(SceneCaptureItem.CaptureThisFrame);

		//CaptureScene을 호출 한 경우 혹은 매 프레임이 Capture인 경우
		if (CaptureThisFrame)
		{
			//SceneCaptureList에 항목을 만들어 서 추가
			RenderingData::SceneCaptureItem RenderSceneCaptureItem;

			RenderSceneCaptureItem.Location = *(SceneCaptureItem.Location);
			RenderSceneCaptureItem.Rotation = *(SceneCaptureItem.Rotation);
			RenderSceneCaptureItem.Scale = *(SceneCaptureItem.Scale);

			RenderSceneCaptureItem.View = *(SceneCaptureItem.View);
			RenderSceneCaptureItem.Projection = *(SceneCaptureItem.Projection);

			RenderSceneCaptureItem.RenderTarget = *(SceneCaptureItem.RenderTarget);
			RenderSceneCaptureItem.DepthStencil = *(SceneCaptureItem.DepthStencil);

			//Hidden Actor 명단을 그대로 복사
			RenderSceneCaptureItem.HiddenActors = *(SceneCaptureItem.HiddenActors);

			this->SceneCaptureList.push_back(RenderSceneCaptureItem);
		}

		//CaptureEveryFrame을 따라감  (true면 계속 true, False면 호출시만 Capture 될꺼임)
		*(SceneCaptureItem.CaptureThisFrame) = CaptureEveryFrame;
	}
}

void Renderer::CreatePlanarReflectionList()
{
	PlanarReflectionList.clear();

	FPPlanarReflectionList* PlanarReflectionRenderList = static_cast<FPPlanarReflectionList*>(FPGameInstance::Get().GetPlanarReflectionList());
	std::vector<PlanarReflectionItem> PlanarReflectionItemList = PlanarReflectionRenderList->GetRenderList();

	for (PlanarReflectionItem& PlanarReflectionItem : PlanarReflectionItemList)
	{
		if (!(*(PlanarReflectionItem.Active))) continue;

		bool CaptureEveryFrame = *(PlanarReflectionItem.CaptureEveryFrame);
		bool CaptureThisFrame = *(PlanarReflectionItem.CaptureThisFrame);

		if (CaptureThisFrame)
		{
			RenderingData::PlanarReflectionItem RenderPlanarReflectionItem;

			RenderPlanarReflectionItem.Location = *(PlanarReflectionItem.Location);
			RenderPlanarReflectionItem.Rotation = *(PlanarReflectionItem.Rotation);
			RenderPlanarReflectionItem.Scale = *(PlanarReflectionItem.Scale);

			RenderPlanarReflectionItem.RenderTarget = *(PlanarReflectionItem.RenderTarget);
			RenderPlanarReflectionItem.DepthStencil = *(PlanarReflectionItem.DepthStencil);

			RenderPlanarReflectionItem.HiddenActors = *(PlanarReflectionItem.HiddenActors);

			RenderPlanarReflectionItem.Plane = *(PlanarReflectionItem.Plane);
			RenderPlanarReflectionItem.ReflectionMatrix = *(PlanarReflectionItem.ReflectionMatrix);

			this->PlanarReflectionList.push_back(RenderPlanarReflectionItem);
		}

		*(PlanarReflectionItem.CaptureThisFrame) = CaptureEveryFrame;
	}
}

void Renderer::PlanarReflectionRenderPass(const RenderingData::CameraItem& CamItem)
{
	if (PlanarReflectionList.empty()) return;

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	bool bAnyDrawn = false;
	std::vector<std::string> UsedDepthStencils;

	for (const RenderingData::PlanarReflectionItem& PlanarReflectionItem : PlanarReflectionList)
	{
		if (!AssetManager->HasRenderTargetData(PlanarReflectionItem.RenderTarget)) continue;
		if (!AssetManager->HastDepthStencilData(PlanarReflectionItem.DepthStencil)) continue;

		FPRenderTargetTexture2DInfo& RTInfo = AssetManager->GetRenderTargetTexture2DInfoData(PlanarReflectionItem.RenderTarget);
		FPRenderTargetData& RTData = AssetManager->GetRenderTargetData(PlanarReflectionItem.RenderTarget);
		FPDepthStencilData& DSData = AssetManager->GetDepthStencilData(PlanarReflectionItem.DepthStencil);

		//카메라가 아닌 장면(Scene)을 반사: ReflectedView = ReflectionMatrix * CamItem.View
		RenderingData::CameraItem ReflectedCamera;
		ReflectedCamera.Location = CamItem.Location;
		ReflectedCamera.Rotation = CamItem.Rotation;
		ReflectedCamera.Scale = CamItem.Scale;
		ReflectedCamera.View = PlanarReflectionItem.ReflectionMatrix * CamItem.View;

		//Proj 재계산
		DirectX::XMFLOAT4X4 ProjFloat;
		DirectX::XMStoreFloat4x4(&ProjFloat, CamItem.Projection.Matrix);
		float RTAspect = (float)RTInfo.SizeX / (float)RTInfo.SizeY;
		ProjFloat._11 = ProjFloat._22 / RTAspect;
		ReflectedCamera.Projection = FPMatrix(DirectX::XMLoadFloat4x4(&ProjFloat));

		//이 RenderTarget의 ReflectionMatrix/Plane을 캐싱 (샘플링하는 forward-reflection Material이 이름으로 조회)
		PlanarReflectionCache[PlanarReflectionItem.RenderTarget] = { PlanarReflectionItem.ReflectionMatrix, PlanarReflectionItem.Plane };

		Device.OMSetRenderTargets(1, RTData.RenderTargetView, DSData.DepthStencilView);
		Device.ClearRenderTargetView(RTData.RenderTargetView, (float*)&RTInfo.ClearColor);

		for (int i = 0; i < 14; ++i)
		{
			Device.VSSetConstantBuffers(i, 1);
			Device.PSSetConstantBuffers(i, 1);
		}

		Device.IASetPrimitiveTopology(Topology::TRIANGLELIST);
		Device.SetViewPort(0.0f, 0.0f, (float)RTInfo.SizeX, (float)RTInfo.SizeY, 0.0f, 1.0f);

		//이 Pass의 모든 Draw에서 거울 평면 반대쪽은 클리핑
		RenderingData::ClipPlanarReflectionConstBuffer ClipCB;
		ClipCB.GlobalClipPlane = PlanarReflectionItem.Plane;
		ClipCB.ClipPlaneEnabled = 1;
		Device.UpdateVertexShaderSubresource(13, reinterpret_cast<const uint8_t*>(&ClipCB), 0, sizeof(RenderingData::ClipPlanarReflectionConstBuffer));

		for (const RenderingData::MeshRenderItem& RenderItem : SkyBoxRenderList)
		{
			if (std::find(PlanarReflectionItem.HiddenActors.begin(), PlanarReflectionItem.HiddenActors.end(), RenderItem.ActorName) != PlanarReflectionItem.HiddenActors.end()) continue;
			DrawMeshRenderItem(RenderItem, ReflectedCamera, AssetManager, "DS_DEPTH_WRITE_OFF", 0, false, false, true, PlanarReflectionItem.Plane);
		}

		for (const RenderingData::MeshRenderItem& RenderItem : OpaqueRenderList)
		{
			if (std::find(PlanarReflectionItem.HiddenActors.begin(), PlanarReflectionItem.HiddenActors.end(), RenderItem.ActorName) != PlanarReflectionItem.HiddenActors.end()) continue;
			DrawMeshRenderItem(RenderItem, ReflectedCamera, AssetManager, "DS_DEPTH_ON", 0, false, true, true, PlanarReflectionItem.Plane);
		}

		for (const RenderingData::MeshRenderItem& RenderItem : MaskedRenderList)
		{
			if (std::find(PlanarReflectionItem.HiddenActors.begin(), PlanarReflectionItem.HiddenActors.end(), RenderItem.ActorName) != PlanarReflectionItem.HiddenActors.end()) continue;
			DrawMeshRenderItem(RenderItem, ReflectedCamera, AssetManager, "DS_DEPTH_ON", 0, false, true, true, PlanarReflectionItem.Plane);
		}

		for (const RenderingData::MeshRenderItem& RenderItem : TranslucentRenderList)
		{
			if (std::find(PlanarReflectionItem.HiddenActors.begin(), PlanarReflectionItem.HiddenActors.end(), RenderItem.ActorName) != PlanarReflectionItem.HiddenActors.end()) continue;
			DrawMeshRenderItem(RenderItem, ReflectedCamera, AssetManager, "DS_DEPTH_WRITE_OFF", 0, false, true, true, PlanarReflectionItem.Plane);
		}

		Device.ResolveRenderTarget(RTInfo.RenderTragetFormat, RTData.Texture, RTData.ResolveTexture);

		bAnyDrawn = true;
		if (std::find(UsedDepthStencils.begin(), UsedDepthStencils.end(), PlanarReflectionItem.DepthStencil) == UsedDepthStencils.end())
		{
			UsedDepthStencils.push_back(PlanarReflectionItem.DepthStencil);
		}
	}

	if (!bAnyDrawn) return;

	//이후 Pass에 영향이 없도록 Clip Plane을 다시 비활성화
	RenderingData::ClipPlanarReflectionConstBuffer DisabledClipCB;
	Device.UpdateVertexShaderSubresource(13, reinterpret_cast<const uint8_t*>(&DisabledClipCB), 0, sizeof(RenderingData::ClipPlanarReflectionConstBuffer));

	//이 Pass가 사용한 Depth/Stencil은 다음 사용 전에 깨끗이 지워둠 (다음 카메라/다음 프레임 캡처에 이전 깊이가 남지 않도록)
	for (const std::string& DepthStencilName : UsedDepthStencils)
	{
		FPDepthStencilData& UsedDSData = AssetManager->GetDepthStencilData(DepthStencilName);
		FPDepthStencilInfo& UsedDSInfo = AssetManager->HasDepthStencilInfoData(DepthStencilName) ? AssetManager->GetDepthStencilInfoData(DepthStencilName) : AssetManager->GetBuiltInDepthStencilInfoData(DepthStencilName);
		Device.ClearDepthStencilView(UsedDSData.DepthStencilView, UsedDSInfo.ClearDepth, (UINT8)UsedDSInfo.ClearStencil);
	}

	Device.OMSetBlendState(AssetManager->GetBlendStateData("BS_DEFAULT").BlendStateData);
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

	hr = Device.CreateBackBufferRenderTarget();
	assert(SUCCEEDED(hr) && "백버퍼 - 렌더타겟 설정 실패\n");

	Device.GetDeviceInfo();

	FontBatch = Device.CreateSpriteBatch();
	Font = Device.CreateSpriteFont();

	shaderFactory = &ShaderFactory::GetShaderFactory();


	//9232B의 ConstBuffer 생성
	Device.CreateVertexShaderConstBuffer(9232);
	Device.CreatePixelShaderConstBuffer(9232);

	return hr;
}

void Renderer::DrawSceneCaptureMeshItem(const RenderingData::MeshRenderItem& RenderItem, const RenderingData::SceneCaptureItem& CaptureItem, FPAssetManager* AssetManager, const std::string& DepthStencilState, const FPMatrix& ReflectionViewProj)
{
	RenderingData::MVPConstBuffer MVPCB;

	//Skip Actors hidden from this Capture
	if (std::find(CaptureItem.HiddenActors.begin(), CaptureItem.HiddenActors.end(), RenderItem.ActorName) != CaptureItem.HiddenActors.end())
	{
		return;
	}

	//Fill/Cull mode Set
	SetRasterizerState(AssetManager, bFill, !(RenderItem.TwoSided));

	//Material이 지정한 DepthStencilState/BlendState 적용 (ZEnable이 꺼져 있으면 디버그용으로 깊이 테스트 끔)
	FPDepthStencilStateData& DepthStencilStateData = AssetManager->GetDepthStencilStateData(ZEnable ? DepthStencilState : "DS_DEPTH_OFF");
	Device.OMSetDepthStencilState(DepthStencilStateData.DepthStencilData);

	FPBlendStateData& BlendStateData = AssetManager->GetBlendStateData(RenderItem.BlendState);
	Device.OMSetBlendState(BlendStateData.BlendStateData);

	std::tuple<void*, void*, void*> VertexShaderData = AssetManager->GetVertexShader(RenderItem.VertexShaderPath);
	std::pair<void*, void*> PixelShaderData = AssetManager->GetPixelShader(RenderItem.PixelShaderPath);

	//Input Layout Set
	Device.IASetInputLayout(std::get<2>(VertexShaderData));

	//Shader Set
	Device.VSSetShader(std::get<0>(VertexShaderData));
	Device.PSSetShader(std::get<0>(PixelShaderData));

	//Shader ConstBuffer Set

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

	//Texture Set
	const std::vector<FPReflectionResourceInfo>& Resources = RenderItem.Resources;

	for (const FPReflectionResourceInfo& TextureItem : Resources)
	{
		void* TextureData = nullptr;
		//Texture Resolve
		if (TextureItem.TextureType == Texture2D) { TextureData = AssetManager->GetTextureData(TextureItem.AssetName).ShaderResourceView; }
		if (TextureItem.TextureType == Texture2DArray) { TextureData = AssetManager->GetTextureArrayData(TextureItem.AssetName).TextureArraySRV; }
		if (TextureItem.TextureType == RenderTargetTexture2D) { TextureData = AssetManager->GetRenderTargetData(TextureItem.AssetName).ShaderResourceView; }
		if (TextureItem.TextureType == DepthStencilTexture2D) { TextureData = AssetManager->GetDepthStencilData(TextureItem.AssetName).ShaderResourceView; }
		if (TextureItem.TextureType == StencilTexture2D) { TextureData = AssetManager->GetDepthStencilData(TextureItem.AssetName).StencilShaderResourceView; }
		if (TextureItem.TextureType == CubeMap) { TextureData = AssetManager->GetTextureData(TextureItem.AssetName).ShaderResourceView; }

		Device.PSSetShaderResources(TextureItem.Slot, 1, TextureData);
	}
	//Texture Set End.

	//Sampler Set
	const std::vector<FPReflectionSamplerInfo>& Samplers = RenderItem.Samplers;

	for (const FPReflectionSamplerInfo& SamplerItem : Samplers)
	{
		Device.PSSetSamplers(SamplerItem.Slot, 1, SamplerItem.SamplerType);
	}
	//Sampler Set End.


	//SkyLight 환경맵 텍스처/샘플러 고정 바인딩 (EnvTex : t127, EnvSampler : s15)
	void* EnvTextureData = nullptr;
	if (!FPSkyLightList.empty() && AssetManager->HasTextureData(FPSkyLightList[0].CubeMap))
	{
		EnvTextureData = AssetManager->GetTextureData(FPSkyLightList[0].CubeMap).ShaderResourceView;
	}
	Device.PSSetShaderResources(127, 1, EnvTextureData);
	Device.PSSetSamplers(15, 1, FPSamplerType::Filter_Tri_Linear_Sampler_CLAMP);
	FPVertexBufferData PositionVB = AssetManager->GetPositionVertexBuffer(RenderItem.MeshPath);
	FPVertexBufferData NormalVB = AssetManager->GetNormalVertexBuffer(RenderItem.MeshPath);
	FPVertexBufferData ColorVB = AssetManager->GetColorVertexBuffer(RenderItem.MeshPath);
	FPVertexBufferData UVVB = AssetManager->GetUVVertexBuffer(RenderItem.MeshPath);
	FPIndexBufferData IndexB = AssetManager->GetIndexBuffer(RenderItem.MeshPath);
	FPVertexBufferData TangentVB = AssetManager->GetTangentVertexBuffer(RenderItem.MeshPath);

	//World/View/Proj 행렬 계산 (HLSL은 행 기준이므로 연산 순서는 HLSL과 반대로)
	MVPCB.WorldMatrix = ((RenderItem.Scale) * (RenderItem.Rotation) * (RenderItem.Location)).Matrix;
	MVPCB.InverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WorldMatrix);
	MVPCB.TranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.WorldMatrix);
	MVPCB.InverseTranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWorldMatrix);

	MVPCB.ViewMatrix = CaptureItem.View.Matrix;
	MVPCB.InverseViewMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.ViewMatrix);
	MVPCB.TranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.ViewMatrix);
	MVPCB.InverseTranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseViewMatrix);

	MVPCB.ProjMatrix = CaptureItem.Projection.Matrix;
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

	//Object ConstBuffer Update
	Device.UpdateVertexShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);
	RenderingData::ClipPlanarReflectionConstBuffer ClipCB;
	ClipCB.ReflectionViewProj = ReflectionViewProj;
	Device.UpdateVertexShaderSubresource(13, reinterpret_cast<const uint8_t*>(&ClipCB), 0, sizeof(RenderingData::ClipPlanarReflectionConstBuffer));
	Device.UpdatePixelShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);

	Device.IASetVertexBuffers(0, 1, PositionVB.VertexBuffer, &(PositionVB.Stride), &(PositionVB.Offset));
	Device.IASetVertexBuffers(1, 1, NormalVB.VertexBuffer, &(NormalVB.Stride), &(NormalVB.Offset));
	Device.IASetVertexBuffers(2, 1, ColorVB.VertexBuffer, &(ColorVB.Stride), &(ColorVB.Offset));
	Device.IASetVertexBuffers(3, 1, UVVB.VertexBuffer, &(UVVB.Stride), &(UVVB.Offset));
	Device.IASetVertexBuffers(4, 1, TangentVB.VertexBuffer, &(TangentVB.Stride), &(TangentVB.Offset));
	Device.IASetIndexBuffers(IndexB.IndexBuffer, IndexB.Offset);

	Device.DrawIndexed(IndexB.Index_Cnt, 0, 0);																//Draw Call
}

void Renderer::SceneCaptureRenderPass()
{
	//BackBuffer RenderTarget / DepthStencil Default Set
	FPDepthStencilData& BackBufferDepthStencilData = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager())->GetDepthStencilData("BackBufferDepthStencil");
	Device.OMSetBackBufferRenderTargets(1, BackBufferDepthStencilData.DepthStencilView);

	//Constant Buffer Slot 14 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	if (SceneCaptureList.empty()) return;

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	//Primitive Topology Set
	Device.IASetPrimitiveTopology(Topology::TRIANGLELIST);

	for (const RenderingData::SceneCaptureItem& CaptureItem : SceneCaptureList)
	{
		//이 SceneCaptureItem의 시점(View/Projection)을 CameraItem으로 변환해 Planar Reflection RT를 먼저 갱신
		RenderingData::CameraItem CaptureCamItem;
		CaptureCamItem.Location = CaptureItem.Location;
		CaptureCamItem.Rotation = CaptureItem.Rotation;
		CaptureCamItem.Scale = CaptureItem.Scale;
		CaptureCamItem.View = CaptureItem.View;
		CaptureCamItem.Projection = CaptureItem.Projection;
		PlanarReflectionRenderPass(CaptureCamItem);

		//PlanarReflectionRenderPass가 바꾼 상태를 이 Capture용으로 복원
		for (int i = 0; i < 14; ++i)
		{
			Device.VSSetConstantBuffers(i, 1);
			Device.PSSetConstantBuffers(i, 1);
		}
		Device.IASetPrimitiveTopology(Topology::TRIANGLELIST);

		if (!AssetManager->HasRenderTargetData(CaptureItem.RenderTarget))
		{
			std::cout << "SceneCapture RenderTarget not loaded: " << CaptureItem.RenderTarget << "\n";
			continue;
		}

		if (!AssetManager->HastDepthStencilData(CaptureItem.DepthStencil))
		{
			std::cout << "SceneCapture DepthStencil not loaded: " << CaptureItem.DepthStencil << "\n";
			continue;
		}

		FPRenderTargetTexture2DInfo& CaptureRTInfo = AssetManager->GetRenderTargetTexture2DInfoData(CaptureItem.RenderTarget);
		FPRenderTargetData& CaptureRTData = AssetManager->GetRenderTargetData(CaptureItem.RenderTarget);
		FPDepthStencilData& CaptureDSData = AssetManager->GetDepthStencilData(CaptureItem.DepthStencil);

		//Capture RenderTarget as the output target
		Device.OMSetRenderTargets(1, CaptureRTData.RenderTargetView, CaptureDSData.DepthStencilView);
		Device.ClearRenderTargetView(CaptureRTData.RenderTargetView, (float*)&CaptureRTInfo.ClearColor);

		Device.SetViewPort(0.0f, 0.0f, (float)CaptureRTInfo.SizeX, (float)CaptureRTInfo.SizeY, 0.0f, 1.0f);

		for (const RenderingData::MeshRenderItem& RenderItem : SkyBoxRenderList)
		{
			DrawSceneCaptureMeshItem(RenderItem, CaptureItem, AssetManager, "DS_DEPTH_WRITE_OFF");
		}

		PlanarReflectionOpaqueSceneCaptureRenderPass(CaptureItem, CaptureCamItem, AssetManager);
		for (const RenderingData::MeshRenderItem& RenderItem : OpaqueRenderList)
		{
			DrawSceneCaptureMeshItem(RenderItem, CaptureItem, AssetManager, "DS_DEPTH_ON");
		}

		PlanarReflectionMaskedSceneCaptureRenderPass(CaptureItem, CaptureCamItem, AssetManager);
		for (const RenderingData::MeshRenderItem& RenderItem : MaskedRenderList)
		{
			DrawSceneCaptureMeshItem(RenderItem, CaptureItem, AssetManager, "DS_DEPTH_ON");
		}

		PlanarReflectionmTranslucentSceneCaptureRenderPass(CaptureItem, CaptureCamItem, AssetManager);
		for (const RenderingData::MeshRenderItem& RenderItem : TranslucentRenderList)
		{
			DrawSceneCaptureMeshItem(RenderItem, CaptureItem, AssetManager, "DS_DEPTH_WRITE_OFF");
		}

		//모두 그렸다면 Resolve <- 반드시..MSAA Texture를 일반 Texture로 변환
		Device.ResolveRenderTarget(CaptureRTInfo.RenderTragetFormat, CaptureRTData.Texture, CaptureRTData.ResolveTexture);
	}

	Device.OMSetBlendState(AssetManager->GetBlendStateData("BS_DEFAULT").BlendStateData);
}

void Renderer::CalculateViewPortSize(float Width, float Height, float& OutTopLeftX, float& OutTopLeftY, float& OutWidth, float& OutHeight)
{
	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	float Aspect = GameProjectSetting->GetDisplayAspect();

	float WindowAspect = Width / Height;

	if (WindowAspect > Aspect)
	{
		OutHeight = Height;
		OutWidth = OutHeight * Aspect;
	}
	else
	{
		OutWidth = Width;
		OutHeight = OutWidth / Aspect;
	}

	OutTopLeftX = (Width - OutWidth) * 0.5f;
	OutTopLeftY = (Height - OutHeight) * 0.5f;
}

void Renderer::DrawMeshRenderItem(const RenderingData::MeshRenderItem& RenderItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager, const std::string& DepthStencilState, UINT StencilRef, bool bDisablePixelShader, bool bFlipCull, bool bClipPlaneEnabled, const FPVector4& ClipPlane, const FPMatrix& ReflectionViewProj)
{
	RenderingData::MVPConstBuffer MVPCB;

	/*
	std::cout << RenderItem.MeshPath << "\n";
	std::cout << RenderItem.TextureList.size() << "\n";
	std::cout << RenderItem.VertexShaderPath << "\n";
	std::cout << RenderItem.PixelShaderPath << "\n";
	*/

	//렌더링 모드 전환
	SetRasterizerState(AssetManager, true, !(RenderItem.TwoSided), bFlipCull);

	//Material이 지정한 DepthStencilState/BlendState 적용 (ZEnable이 꺼져 있으면 디버그용으로 깊이 테스트 끔)
	FPDepthStencilStateData& DepthStencilStateData = AssetManager->GetDepthStencilStateData(ZEnable ? DepthStencilState : "DS_DEPTH_OFF");
	Device.OMSetDepthStencilState(DepthStencilStateData.DepthStencilData, StencilRef);

	FPBlendStateData& BlendStateData = AssetManager->GetBlendStateData(RenderItem.BlendState);
	Device.OMSetBlendState(BlendStateData.BlendStateData);

	std::tuple<void*, void*, void*> VertexShaderData = AssetManager->GetVertexShader(RenderItem.VertexShaderPath);

	//입력 레이아웃 설정
	Device.IASetInputLayout(std::get<2>(VertexShaderData));

	//Shader 설정
	Device.VSSetShader(std::get<0>(VertexShaderData));
	if (bDisablePixelShader)
	{
		Device.PSSetShader(nullptr);
	}
	else
	{
		std::pair<void*, void*> PixelShaderData = AssetManager->GetPixelShader(RenderItem.PixelShaderPath);
		Device.PSSetShader(std::get<0>(PixelShaderData));
	}

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
		if (TextureItem.TextureType == Texture2D) { TextureData = AssetManager->GetTextureData(TextureItem.AssetName).ShaderResourceView; }
		if (TextureItem.TextureType == Texture2DArray) { TextureData = AssetManager->GetTextureArrayData(TextureItem.AssetName).TextureArraySRV; }
		if (TextureItem.TextureType == RenderTargetTexture2D) { TextureData = AssetManager->GetRenderTargetData(TextureItem.AssetName).ShaderResourceView; }
		if (TextureItem.TextureType == DepthStencilTexture2D) { TextureData = AssetManager->GetDepthStencilData(TextureItem.AssetName).ShaderResourceView; }
		if (TextureItem.TextureType == StencilTexture2D) { TextureData = AssetManager->GetDepthStencilData(TextureItem.AssetName).StencilShaderResourceView; }
		if (TextureItem.TextureType == CubeMap) { TextureData = AssetManager->GetTextureData(TextureItem.AssetName).ShaderResourceView; }

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

	//SkyLight 환경맵 텍스처/샘플러 고정 바인딩 (EnvTex : t127, EnvSampler : s15)
	void* EnvTextureData = nullptr;
	if (!FPSkyLightList.empty() && AssetManager->HasTextureData(FPSkyLightList[0].CubeMap))
	{
		EnvTextureData = AssetManager->GetTextureData(FPSkyLightList[0].CubeMap).ShaderResourceView;
	}
	Device.PSSetShaderResources(127, 1, EnvTextureData);
	Device.PSSetSamplers(15, 1, FPSamplerType::Filter_Tri_Linear_Sampler_CLAMP);
	FPVertexBufferData PositionVB = AssetManager->GetPositionVertexBuffer(RenderItem.MeshPath);
	FPVertexBufferData NormalVB = AssetManager->GetNormalVertexBuffer(RenderItem.MeshPath);
	FPVertexBufferData ColorVB = AssetManager->GetColorVertexBuffer(RenderItem.MeshPath);
	FPVertexBufferData UVVB = AssetManager->GetUVVertexBuffer(RenderItem.MeshPath);
	FPIndexBufferData IndexB = AssetManager->GetIndexBuffer(RenderItem.MeshPath);
	FPVertexBufferData TangentVB = AssetManager->GetTangentVertexBuffer(RenderItem.MeshPath);
	/*
	std::cout << "<VB Size>" << "\n";
	std::cout << PositionVB.VB_Cnt << "\n";
	std::cout << NormalVB.VB_Cnt << "\n";
	std::cout << ColorVB.VB_Cnt << "\n";
	std::cout << UVVB.VB_Cnt << "\n";
	std::cout << IndexB.Index_Cnt << "\n";
	*/

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
	RenderingData::ClipPlanarReflectionConstBuffer ClipCB;
	ClipCB.GlobalClipPlane = ClipPlane;
	ClipCB.ClipPlaneEnabled = bClipPlaneEnabled ? 1 : 0;
	ClipCB.ReflectionViewProj = ReflectionViewProj;
	Device.UpdateVertexShaderSubresource(13, reinterpret_cast<const uint8_t*>(&ClipCB), 0, sizeof(RenderingData::ClipPlanarReflectionConstBuffer));
	Device.UpdatePixelShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);

	Device.IASetVertexBuffers(0, 1, PositionVB.VertexBuffer, &(PositionVB.Stride), &(PositionVB.Offset));
	Device.IASetVertexBuffers(1, 1, NormalVB.VertexBuffer, &(NormalVB.Stride), &(NormalVB.Offset));
	Device.IASetVertexBuffers(2, 1, ColorVB.VertexBuffer, &(ColorVB.Stride), &(ColorVB.Offset));
	Device.IASetVertexBuffers(3, 1, UVVB.VertexBuffer, &(UVVB.Stride), &(UVVB.Offset));
	Device.IASetVertexBuffers(4, 1, TangentVB.VertexBuffer, &(TangentVB.Stride), &(TangentVB.Offset));
	Device.IASetIndexBuffers(IndexB.IndexBuffer, IndexB.Offset);

	Device.DrawIndexed(IndexB.Index_Cnt, 0, 0);																//Draw Call
}

void Renderer::PreDepthStencilRenderPass()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	//MeshRenderPass와 같은 Letterbox ViewPort를 사용해 Pre-Pass 깊이가 색상 Pass와 일치하도록 함
	FPRenderTargetTexture2DInfo& RTInfo = AssetManager->GetBuiltInRenderTargetTexture2DInfoData("MeshRenderPass-RT-BuiltIn");
	float ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight;
	CalculateViewPortSize((float)RTInfo.SizeX, (float)RTInfo.SizeY, ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight);
	Device.SetViewPort(ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight, 0.0f, 1.0f);

	//Topology / 상수 버퍼 슬롯은 이전 프레임의 마지막 Pass가 설정한 값을 보장할 수 없으므로 직접 설정
	Device.IASetPrimitiveTopology(Topology::TRIANGLELIST);
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	for (RenderingData::CameraItem& CamItem : CameraList)
	{
		PreDepthStencilPass(CamItem, AssetManager);
		CustomDepthStencilPass(CamItem, AssetManager);
	}
}

void Renderer::PreDepthStencilPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	FPDepthStencilData& CustomDepthStencilData = AssetManager->GetDepthStencilData("BackBufferDepthStencil");
	Device.OMSetRenderTargets(0, nullptr, CustomDepthStencilData.DepthStencilView);

	for (const RenderingData::MeshRenderItem& RenderItem : OpaqueRenderList)
	{
		DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON", 0, true);
	}

	/* Masked에 대한 처리는 나중에
	for (const RenderingData::MeshRenderItem& RenderItem : MaskedRenderList)
	{
		if (!RenderItem.bCustomDepthStencil) continue;
		DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON_STENCIL_ON", (UINT)RenderItem.StencilRefValue, true);
	}
	*/
}

void Renderer::CustomDepthStencilPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	FPDepthStencilData& CustomDepthStencilData = AssetManager->GetDepthStencilData("CustomDepthStencil");
	Device.OMSetRenderTargets(0, nullptr, CustomDepthStencilData.DepthStencilView);

	for (const RenderingData::MeshRenderItem& RenderItem : OpaqueRenderList)
	{
		if (!RenderItem.bCustomDepthStencil || RenderItem.StencilRefValue == 1) continue;
		DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON_STENCIL_ON", (UINT)RenderItem.StencilRefValue, true);
	}

	/* Masked / Translucent에 대한 처리는 나중에
	for (const RenderingData::MeshRenderItem& RenderItem : MaskedRenderList)
	{
		if (!RenderItem.bCustomDepthStencil) continue;
		DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON_STENCIL_ON", (UINT)RenderItem.StencilRefValue, true);
	}
	*/
}

void Renderer::SkyBoxRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : SkyBoxRenderList)
	{
		DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_WRITE_OFF");
	}
}

void Renderer::OpaqueRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : OpaqueRenderList)
	{
		if (RenderItem.bCustomDepthStencil)
		{
			DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON_STENCIL_ON", (UINT)RenderItem.StencilRefValue);
		}
		else
		{
			DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON");
		}
	}
}

void Renderer::MaskedRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : MaskedRenderList)
	{
		DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON");
	}
}

void Renderer::TranslucentRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : TranslucentRenderList)
	{
		DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_WRITE_OFF");
	}
}

FPMatrix Renderer::ComputePlanarReflectionViewProj(const RenderingData::MeshRenderItem& RenderItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	//해당 이름으로 캐싱된 ReflectionMatrix/Plane이 있는, RenderTarget 타입의 바인딩된 PS 텍스처를 탐색
	for (const FPReflectionResourceInfo& TextureItem : RenderItem.Resources)
	{
		if (TextureItem.TextureType != RenderTargetTexture2D) continue;

		auto CacheIt = PlanarReflectionCache.find(TextureItem.AssetName);
		if (CacheIt == PlanarReflectionCache.end()) continue;
		if (!AssetManager->HasRenderTargetData(TextureItem.AssetName)) continue;

		FPRenderTargetTexture2DInfo& RTInfo = AssetManager->GetRenderTargetTexture2DInfoData(TextureItem.AssetName);

		//이 RenderTarget을 캡처할 때 쓴 화면비 보정 Projection과 동일하게 맞춰, 텍스처 내용과 UV가 일치하도록 함
		DirectX::XMFLOAT4X4 ProjFloat;
		DirectX::XMStoreFloat4x4(&ProjFloat, CamItem.Projection.Matrix);
		float RTAspect = (float)RTInfo.SizeX / (float)RTInfo.SizeY;
		ProjFloat._11 = ProjFloat._22 / RTAspect;
		FPMatrix AspectCorrectedProj = FPMatrix(DirectX::XMLoadFloat4x4(&ProjFloat));

		return CacheIt->second.ReflectionMatrix * CamItem.View * AspectCorrectedProj;
	}

	return FPMatrix();
}

void Renderer::PlanarReflectionOpaqueRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : PlanarReflectionOpaqueRenderList)
	{
		FPMatrix ReflectionViewProj = ComputePlanarReflectionViewProj(RenderItem, CamItem, AssetManager);

		if (RenderItem.bCustomDepthStencil)
		{
			DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON_STENCIL_ON", (UINT)RenderItem.StencilRefValue, false, false, false, FPVector4(), ReflectionViewProj);
		}
		else
		{
			DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON", 0, false, false, false, FPVector4(), ReflectionViewProj);
		}
	}
}

void Renderer::PlanarReflectionMaskedRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : PlanarReflectionMaskedRenderList)
	{
		FPMatrix ReflectionViewProj = ComputePlanarReflectionViewProj(RenderItem, CamItem, AssetManager);
		DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_ON", 0, false, false, false, FPVector4(), ReflectionViewProj);
	}
}

void Renderer::PlanarReflectionmTranslucentRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : PlanarReflectionmTranslucentRenderList)
	{
		FPMatrix ReflectionViewProj = ComputePlanarReflectionViewProj(RenderItem, CamItem, AssetManager);
		DrawMeshRenderItem(RenderItem, CamItem, AssetManager, "DS_DEPTH_WRITE_OFF", 0, false, false, false, FPVector4(), ReflectionViewProj);
	}
}

void Renderer::PlanarReflectionOpaqueSceneCaptureRenderPass(const RenderingData::SceneCaptureItem& CaptureItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : PlanarReflectionOpaqueRenderList)
	{
		FPMatrix ReflectionViewProj = ComputePlanarReflectionViewProj(RenderItem, CamItem, AssetManager);
		DrawSceneCaptureMeshItem(RenderItem, CaptureItem, AssetManager, "DS_DEPTH_ON", ReflectionViewProj);
	}
}

void Renderer::PlanarReflectionMaskedSceneCaptureRenderPass(const RenderingData::SceneCaptureItem& CaptureItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : PlanarReflectionMaskedRenderList)
	{
		FPMatrix ReflectionViewProj = ComputePlanarReflectionViewProj(RenderItem, CamItem, AssetManager);
		DrawSceneCaptureMeshItem(RenderItem, CaptureItem, AssetManager, "DS_DEPTH_ON", ReflectionViewProj);
	}
}

void Renderer::PlanarReflectionmTranslucentSceneCaptureRenderPass(const RenderingData::SceneCaptureItem& CaptureItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager)
{
	for (const RenderingData::MeshRenderItem& RenderItem : PlanarReflectionmTranslucentRenderList)
	{
		FPMatrix ReflectionViewProj = ComputePlanarReflectionViewProj(RenderItem, CamItem, AssetManager);
		DrawSceneCaptureMeshItem(RenderItem, CaptureItem, AssetManager, "DS_DEPTH_WRITE_OFF", ReflectionViewProj);
	}
}

void Renderer::MeshRenderPass()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	std::string MeshRenderPassRT = "MeshRenderPass-RT-BuiltIn";
	SceneRenderTarget = MeshRenderPassRT;		//이번 프레임의 장면 색상 RT (PostProcessRenderPass가 바꿀 수 있음)

	//BackBuffer RenderTarget / DepthStencil Default Set
	FPRenderTargetTexture2DInfo& RTInfo = AssetManager->GetBuiltInRenderTargetTexture2DInfoData(MeshRenderPassRT);
	FPRenderTargetData& RTData = AssetManager->GetRenderTargetData(MeshRenderPassRT);
	FPDepthStencilData& BackBufferDepthStencilData = AssetManager->GetDepthStencilData("BackBufferDepthStencil");

	//MeshRenderPass RanderTarget Setting
	Device.OMSetRenderTargets(1, RTData.RenderTargetView, BackBufferDepthStencilData.DepthStencilView);
	Device.ClearRenderTargetView(RTData.RenderTargetView, (float*)&RTInfo.ClearColor);


	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	//기하 위상 구조 설정
	Device.IASetPrimitiveTopology(Topology::TRIANGLELIST);

	//상수 버퍼 설정
	//Vertex Shader
	//MeshRenderPass에서 0번은 WVP, 1번은 ViewPort용 슬롯, 2번은 Light용 슬롯 임
	//PixelShader
	//MeshRenderPass에서 0번은 ViewPort용 슬롯


	float ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight;
	CalculateViewPortSize((float)RTInfo.SizeX, (float)RTInfo.SizeY, ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight);
	Device.SetViewPort(ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight, 0.0f, 1.0f);

	for (RenderingData::CameraItem& CamItem : CameraList)
	{
		//이 CamItem 시점 기준으로 Planar Reflection RT를 먼저 갱신 (MeshRenderPass의 RT/ViewPort 상태를 바꾸므로 이후 복원)
		PlanarReflectionRenderPass(CamItem);

		Device.OMSetRenderTargets(1, RTData.RenderTargetView, BackBufferDepthStencilData.DepthStencilView);

		for (int i = 0; i < 14; ++i)
		{
			Device.VSSetConstantBuffers(i, 1);
			Device.PSSetConstantBuffers(i, 1);
		}

		Device.IASetPrimitiveTopology(Topology::TRIANGLELIST);
		Device.SetViewPort(ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight, 0.0f, 1.0f);

		SkyBoxRenderPass(CamItem, AssetManager);

		PlanarReflectionOpaqueRenderPass(CamItem, AssetManager);
		OpaqueRenderPass(CamItem, AssetManager);

		PlanarReflectionMaskedRenderPass(CamItem, AssetManager);
		MaskedRenderPass(CamItem, AssetManager);

		PlanarReflectionmTranslucentRenderPass(CamItem, AssetManager);
		TranslucentRenderPass(CamItem, AssetManager);

		Device.ResolveRenderTarget(RTInfo.RenderTragetFormat, RTData.Texture, RTData.ResolveTexture);
	}

	Quad2DRenderPass(MeshRenderPassRT);

	Device.OMSetBlendState(AssetManager->GetBlendStateData("BS_DEFAULT").BlendStateData);
}


void Renderer::GizmoRenderPass()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	std::string RenderPassRT = SceneRenderTarget;		//Mesh RT, PostProcess가 적용됐다면 PostProcess RT

	//BackBuffer RenderTarget / DepthStencil Default Set
	FPRenderTargetTexture2DInfo& RTInfo = AssetManager->GetBuiltInRenderTargetTexture2DInfoData(RenderPassRT);
	FPRenderTargetData& RTData = AssetManager->GetRenderTargetData(RenderPassRT);
	FPDepthStencilData& BackBufferDepthStencilData = AssetManager->GetDepthStencilData("BackBufferDepthStencil");

	//MeshRenderPass RanderTarget Setting
	Device.OMSetRenderTargets(1, RTData.RenderTargetView, BackBufferDepthStencilData.DepthStencilView);

	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	//Depth Dtencill 상태 설정
	FPDepthStencilStateData& DepthStencilStateData = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager())->GetDepthStencilStateData(ZEnable ? "DS_DEPTH_ON" : "DS_DEPTH_OFF");
	Device.OMSetDepthStencilState(DepthStencilStateData.DepthStencilData);

	//렌더링 모드 전환
	SetRasterizerState(AssetManager, bFill, bCull);

	//기하 위상 구조 설정
	Device.IASetPrimitiveTopology(Topology::LINELIST);

	//상수 버퍼 설정
	//Vertex Shader
	//MeshRenderPass에서 0번은 WVP, 1번은 ViewPort용 슬롯 임
	//Slot is the absolute register number (b#) in all passes: b0 Object, b1 ViewPort, b2 Light / NormalDebug

	//PixelShader
	//MeshRenderPass에서 0번은 ViewPort용 슬롯


	RenderingData::MVPConstBuffer MVPCB;

	float ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight;
	CalculateViewPortSize((float)RTInfo.SizeX, (float)RTInfo.SizeY, ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight);
	Device.SetViewPort(ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight, 0.0f, 1.0f);

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
			RenderingData::ClipPlanarReflectionConstBuffer ClipCB;
			Device.UpdateVertexShaderSubresource(13, reinterpret_cast<const uint8_t*>(&ClipCB), 0, sizeof(RenderingData::ClipPlanarReflectionConstBuffer));

			Device.IASetVertexBuffers(0, 1, PositionVB.VertexBuffer, &(PositionVB.Stride), &(PositionVB.Offset));
			Device.IASetVertexBuffers(2, 1, ColorVB.VertexBuffer, &(ColorVB.Stride), &(ColorVB.Offset));

			Device.Draw(PositionVB.VB_Cnt, 0);																		//Draw Call
		}

		Device.ResolveRenderTarget(RTInfo.RenderTragetFormat, RTData.Texture, RTData.ResolveTexture);
	}

	Quad2DRenderPass(RenderPassRT);

	Device.OMSetBlendState(AssetManager->GetBlendStateData("BS_DEFAULT").BlendStateData);
}

void Renderer::NormalRenderPass()
{
	if (!bNormal) return;

	//BackBuffer RenderTarget / DepthStencil Default Set
	FPDepthStencilData& BackBufferDepthStencilData = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager())->GetDepthStencilData("BackBufferDepthStencil");
	Device.OMSetBackBufferRenderTargets(1, BackBufferDepthStencilData.DepthStencilView);

	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	//Depth Dtencill 상태 설정
	FPDepthStencilStateData& DepthStencilStateData = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager())->GetDepthStencilStateData(ZEnable ? "DS_DEPTH_ON" : "DS_DEPTH_OFF");
	Device.OMSetDepthStencilState(DepthStencilStateData.DepthStencilData);

	//렌더링 모드 전환
	SetRasterizerState(static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager()), bFill, bCull);

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
	FPDepthStencilInfo& BackBufferDepthStencilInfo = AssetManager->GetBuiltInDepthStencilInfoData("BackBufferDepthStencil");
	float ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight;
	CalculateViewPortSize((float)BackBufferDepthStencilInfo.SizeX, (float)BackBufferDepthStencilInfo.SizeY, ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight);
	Device.SetViewPort(ViewPortTopLeftX, ViewPortTopLeftY, ViewPortWidth, ViewPortHeight, 0.0f, 1.0f);

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
			RenderingData::ClipPlanarReflectionConstBuffer ClipCB;
			Device.UpdateVertexShaderSubresource(13, reinterpret_cast<const uint8_t*>(&ClipCB), 0, sizeof(RenderingData::ClipPlanarReflectionConstBuffer));

			Device.IASetVertexBuffers(0, 1, PositionVB.VertexBuffer, &(PositionVB.Stride), &(PositionVB.Offset));
			Device.IASetVertexBuffers(1, 1, NormalVB.VertexBuffer, &(NormalVB.Stride), &(NormalVB.Offset));
			Device.IASetIndexBuffers(IndexB.IndexBuffer, IndexB.Offset);

			Device.DrawInstanced(2, PositionVB.VB_Cnt, 0, 0);																		//Draw Call
		}
	}
}

void Renderer::UIRenderPass()
{
	//BackBuffer RenderTarget / DepthStencil Default Set
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	FPDepthStencilData& BackBufferDepthStencilData = AssetManager->GetDepthStencilData("BackBufferDepthStencil");
	FPDepthStencilInfo& BackBufferDepthStencilInfo = AssetManager->GetBuiltInDepthStencilInfoData("BackBufferDepthStencil");
	Device.OMSetBackBufferRenderTargets(1, BackBufferDepthStencilData.DepthStencilView);

	//상수 버퍼 14개씩 Set
	for (int i = 0; i < 14; ++i)
	{
		Device.VSSetConstantBuffers(i, 1);
		Device.PSSetConstantBuffers(i, 1);
	}

	Device.SetViewPort(0.0f, 0.0f, (float)BackBufferDepthStencilInfo.SizeX, (float)BackBufferDepthStencilInfo.SizeY, 0.0f, 1.0f);

	for (const RenderingData::UIContextItem& UIData : UIRenderList)
	{
		FontBatch->Begin();

		XMFLOAT4 Color = { (UIData.color.x), (UIData.color.y), (UIData.color.z), (UIData.color.w) };
		XMFLOAT2 Position = { (float)((UIData.x)), (float)((UIData.y)) };
		Font->DrawString(FontBatch, UIData.msg.c_str(), Position, XMLoadFloat4(&Color));

		FontBatch->End();
	}

}


HRESULT Renderer::Finalize()
{
	FontRelease();
	HRESULT hr = Device.DeviceFinalize();


	return hr;
}

RenderingDevice& Renderer::GetRenderingDevice()
{
	return Device;
}

void Renderer::ClearDepthStencil()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	FPDepthStencilData& BackBufferDepthStencilData = AssetManager->GetDepthStencilData("BackBufferDepthStencil");
	FPDepthStencilInfo& BackBufferDepthStencilInfo = AssetManager->GetBuiltInDepthStencilInfoData("BackBufferDepthStencil");

	Device.ClearBackBuffer(BackBufferDepthStencilData.DepthStencilView, BackBufferDepthStencilInfo.ClearDepth, (UINT8)BackBufferDepthStencilInfo.ClearStencil);

	for (auto& [DepthStencilName, DepthStencilData] : AssetManager->GetDepthStencilDataMap())
	{
		if (DepthStencilName == "BackBufferDepthStencil") continue;

		FPDepthStencilInfo& DepthStencilInfo = AssetManager->HasDepthStencilInfoData(DepthStencilName) ? AssetManager->GetDepthStencilInfoData(DepthStencilName) : AssetManager->GetBuiltInDepthStencilInfoData(DepthStencilName);
		Device.ClearDepthStencilView(DepthStencilData.DepthStencilView, DepthStencilInfo.ClearDepth, (UINT8)DepthStencilInfo.ClearStencil);
	}
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

	GameProjectSetting->SyncGameSettingData();

	//새 BackBuffer로 RTV 생성
	HRESULT hr = S_OK;
	hr = Device.CreateBackBufferRenderTarget();
	assert(SUCCEEDED(hr) && "백버퍼 - 렌더타겟 설정 실패\n");

	//새 Depth Buffer생성
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	//BuiltIn Render Target / DepthStencil 전체 재생성 (BackBufferDepthStencil 포함)
	AssetManager->ResizeBuiltInRenderTargetAndDepthStencil();

	FPDepthStencilData& BackBufferDepthStencilData = AssetManager->GetDepthStencilData("BackBufferDepthStencil");
	Device.OMSetBackBufferRenderTargets(1, BackBufferDepthStencilData.DepthStencilView);
}


//Quad2D MVP (Quad2DRenderPass와 PostProcessRenderPass가 공유)
void Renderer::MakeQuad2DMVP(RenderingData::MVPConstBuffer& MVPCB, float DisplayWidth, float DisplayHeight)
{
	//View는 단위 행렬 사용
	FPMatrix View = MatrixIdentity();

	//Ortho-Off Projection 행렬
	FPMatrix Proj = MatrixOrthographicOffCenterLH(0, (float)DisplayWidth,(float)DisplayHeight, 0.0f, 1.0f, 100.0f);

	//화면 해상도 대응 : 렌더타겟과 동일, 화면 중앙 이동
	FPMatrix Scale = MatrixScaling((float)DisplayWidth, (float)DisplayHeight, 1);
	FPMatrix Rotation = MatrixIdentity();
	FPMatrix Location = MatrixTranslation(DisplayWidth/2, DisplayHeight/2, 2.0f);

	////World/View/Proj 행렬 설정 (HLSL과 곱셈 순서가 반대)
	MVPCB.WorldMatrix = (Scale * Rotation * Location).Matrix;
	MVPCB.InverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.WorldMatrix);
	MVPCB.TranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.WorldMatrix);
	MVPCB.InverseTranseposeWorldMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseWorldMatrix);

	MVPCB.ViewMatrix = View.Matrix;
	MVPCB.InverseViewMatrix = DirectX::XMMatrixInverse(nullptr, MVPCB.ViewMatrix);
	MVPCB.TranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.ViewMatrix);
	MVPCB.InverseTranseposeViewMatrix = DirectX::XMMatrixTranspose(MVPCB.InverseViewMatrix);

	MVPCB.ProjMatrix = Proj.Matrix;
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

	//MakeQuad2DMVP 끝
}

//PostProcess 목록 (활성 상태이고 PostProcess Material을 가진 Camera)
void Renderer::CreatePostProcessList()
{
	PostProcessList.clear();

	FPCameraList* CameraRenderList = static_cast<FPCameraList*>(FPGameInstance::Get().GetCameraList());
	std::vector<CameraItem> CamList = CameraRenderList->GetRenderList();

	for (CameraItem& CamItem : CamList)
	{
		if (!(*(CamItem.Active))) continue;
		if (CamItem.PostProcessMaterial == nullptr) continue;
		if (*(CamItem.PostProcessMaterial) == nullptr) continue;

		RenderingData::PostProcessItem PostProcessItem;
		PostProcessItem.Material = *(CamItem.PostProcessMaterial);

		PostProcessList.push_back(PostProcessItem);
	}
}

//PostProcess : Mesh RT -> (PostProcess Material의 PS, DefaultQuad2DVertexShader) -> PostProcess RT -> Quad2DRenderPass -> BackBuffer
void Renderer::PostProcessRenderPass()
{
	if (PostProcessList.empty()) return;

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	const std::string PostProcessRT = "PostProcessRenderPass-RT-BuiltIn";

	if (!AssetManager->HasRenderTargetData(PostProcessRT))
	{
		std::cout << "PostProcess RenderTarget not loaded: " << PostProcessRT << "\n";
		return;
	}

	FPRenderTargetTexture2DInfo& RTInfo = AssetManager->GetBuiltInRenderTargetTexture2DInfoData(PostProcessRT);
	FPRenderTargetData& RTData = AssetManager->GetRenderTargetData(PostProcessRT);

	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	float DisplayWidth = GameProjectSetting->GetDisplayWidth();
	float DisplayHeight = GameProjectSetting->GetDeisplayHeight();

	bool bDrawn = false;

	for (const RenderingData::PostProcessItem& PostProcessItem : PostProcessList)
	{
		//Camera가 가진 PostProcess Material
		FPMaterialInterface* Material = PostProcessItem.Material;
		if (Material == nullptr) continue;

		//출력 : PostProcess RT만 사용 (깊이 버퍼 없음)
		Device.OMSetRenderTargets(1, RTData.RenderTargetView, nullptr);

		//상수 버퍼 14개 슬롯 바인딩
		for (int i = 0; i < 14; ++i)
		{
			Device.VSSetConstantBuffers(i, 1);
			Device.PSSetConstantBuffers(i, 1);
		}

		//렌더 상태 : 컬링 없음, Solid, 블렌드 없음, 깊이 없음
		SetRasterizerState(AssetManager, true, false);
		Device.OMSetBlendState(AssetManager->GetBlendStateData("BS_DEFAULT").BlendStateData);
		Device.OMSetDepthStencilState(AssetManager->GetDepthStencilStateData("DS_DEPTH_OFF").DepthStencilData);

		Device.IASetPrimitiveTopology(Topology::TRIANGLESTRIP);
		Device.SetViewPort(0.0f, 0.0f, (float)RTInfo.SizeX, (float)RTInfo.SizeY, 0.0f, 1.0f);

		//셰이더 : Vertex Shader는 고정, Pixel Shader는 Material에서 가져옴
		std::tuple<void*, void*, void*> VertexShaderData = AssetManager->GetVertexShader("DefaultQuad2DVertexShader");
		std::pair<void*, void*> PixelShaderData = AssetManager->GetPixelShader(*(Material->GetPixelShader()));

		Device.IASetInputLayout(std::get<2>(VertexShaderData));
		Device.VSSetShader(std::get<0>(VertexShaderData));
		Device.PSSetShader(std::get<0>(PixelShaderData));

		//Object 상수 버퍼 (슬롯 0), Quad2DRenderPass와 동일
		RenderingData::MVPConstBuffer MVPCB;
		MakeQuad2DMVP(MVPCB, DisplayWidth, DisplayHeight);
		Device.UpdateVertexShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);
		RenderingData::ClipPlanarReflectionConstBuffer ClipCB;
		Device.UpdateVertexShaderSubresource(13, reinterpret_cast<const uint8_t*>(&ClipCB), 0, sizeof(RenderingData::ClipPlanarReflectionConstBuffer));
		Device.UpdatePixelShaderSubresource(0, reinterpret_cast<const uint8_t*>(&MVPCB), 0, 1280);

		//Material 상수 버퍼
		FPConstantBufferRenderData* VertexConstBuffer = Material->GetVertexConstantBufferRenderData();
		for (FPConstantBufferInfo ConstantBufferInfo : VertexConstBuffer->ConstantBuffers)
		{
			Device.UpdateVertexShaderSubresource(ConstantBufferInfo.Slot, VertexConstBuffer->Buffer.data(), ConstantBufferInfo.Offset, ConstantBufferInfo.Size);
		}

		FPConstantBufferRenderData* PixelConstBuffer = Material->GetPixelConstantBufferRenderData();
		for (FPConstantBufferInfo ConstantBufferInfo : PixelConstBuffer->ConstantBuffers)
		{
			Device.UpdatePixelShaderSubresource(ConstantBufferInfo.Slot, PixelConstBuffer->Buffer.data(), ConstantBufferInfo.Offset, ConstantBufferInfo.Size);
		}

		//Material JSON에 지정된 텍스처 / 샘플러
		const std::vector<FPReflectionResourceInfo>& Resources = Material->GetReflectionData()->Resources;
		for (const FPReflectionResourceInfo& TextureItem : Resources)
		{
			void* TextureData = nullptr;
			if (TextureItem.TextureType == Texture2D) { TextureData = AssetManager->GetTextureData(TextureItem.AssetName).ShaderResourceView; }
			if (TextureItem.TextureType == Texture2DArray) { TextureData = AssetManager->GetTextureArrayData(TextureItem.AssetName).TextureArraySRV; }
			if (TextureItem.TextureType == RenderTargetTexture2D) { TextureData = AssetManager->GetRenderTargetData(TextureItem.AssetName).ShaderResourceView; }
			if (TextureItem.TextureType == DepthStencilTexture2D) { TextureData = AssetManager->GetDepthStencilData(TextureItem.AssetName).ShaderResourceView; }
			if (TextureItem.TextureType == StencilTexture2D) { TextureData = AssetManager->GetDepthStencilData(TextureItem.AssetName).StencilShaderResourceView; }
			if (TextureItem.TextureType == CubeMap) { TextureData = AssetManager->GetTextureData(TextureItem.AssetName).ShaderResourceView; }

			Device.PSSetShaderResources(TextureItem.Slot, 1, TextureData);
		}

		const std::vector<FPReflectionSamplerInfo>& Samplers = Material->GetReflectionData()->Samplers;
		for (const FPReflectionSamplerInfo& SamplerItem : Samplers)
		{
			Device.PSSetSamplers(SamplerItem.Slot, 1, SamplerItem.SamplerType);
		}

		//Quad Mesh 설정
		FPVertexBufferData PositionVB = AssetManager->GetPositionVertexBuffer("Quad2D_Mesh");
		FPVertexBufferData NormalVB = AssetManager->GetNormalVertexBuffer("Quad2D_Mesh");
		FPVertexBufferData ColorVB = AssetManager->GetColorVertexBuffer("Quad2D_Mesh");
		FPVertexBufferData UVVB = AssetManager->GetUVVertexBuffer("Quad2D_Mesh");
		FPIndexBufferData IndexB = AssetManager->GetIndexBuffer("Quad2D_Mesh");
		FPVertexBufferData TangentVB = AssetManager->GetTangentVertexBuffer("Quad2D_Mesh");

		Device.IASetVertexBuffers(0, 1, PositionVB.VertexBuffer, &(PositionVB.Stride), &(PositionVB.Offset));
		Device.IASetVertexBuffers(1, 1, NormalVB.VertexBuffer, &(NormalVB.Stride), &(NormalVB.Offset));
		Device.IASetVertexBuffers(2, 1, ColorVB.VertexBuffer, &(ColorVB.Stride), &(ColorVB.Offset));
		Device.IASetVertexBuffers(3, 1, UVVB.VertexBuffer, &(UVVB.Stride), &(UVVB.Offset));
		Device.IASetVertexBuffers(4, 1, TangentVB.VertexBuffer, &(TangentVB.Stride), &(TangentVB.Offset));
		Device.IASetIndexBuffers(IndexB.IndexBuffer, IndexB.Offset);

		Device.DrawIndexed(IndexB.Index_Cnt, 0, 0);

		//MSAA RT -> Resolve Texture (Quad2DRenderPass / Material은 Resolve Texture를 읽음)
		Device.ResolveRenderTarget(RTInfo.RenderTragetFormat, RTData.Texture, RTData.ResolveTexture);

		//텍스처 바인딩을 해제해 다시 렌더 타겟으로 쓸 수 있게 함
		for (const FPReflectionResourceInfo& TextureItem : Resources)
		{
			Device.PSSetShaderResources(TextureItem.Slot, 1, nullptr);
		}

		bDrawn = true;
	}

	if (!bDrawn) return;

	//이후부터 PostProcess RT가 장면 색상 (GizmoRenderPass가 여기에 그리고 출력함)
	SceneRenderTarget = PostProcessRT;
	Quad2DRenderPass(SceneRenderTarget);
}
