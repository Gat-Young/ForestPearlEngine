#pragma once
#include <vector>
#include <queue>
#include "RenderingDevice.h"
#include "../Shader/ShaderFactory.h"

class FPAssetManager;

namespace RenderingData
{
	struct MeshRenderItem;
	struct GizmoRenderItem;
	struct NormalRenderItem;
	struct CameraItem;
	struct UIContextItem;
	struct MeshRenderItemCompare;
	struct GizmoRenderItemCompare;
	struct UIRenderItemCompare;
	struct LightRenderItem;
	struct FPSkyLightRenderItem;
}

class Renderer
{
	private:
		RenderingDevice& Device;

		//Font
		SpriteBatch* FontBatch = nullptr;
		SpriteFont* Font = nullptr;

		//셰이더 팩토리
		ShaderFactory* shaderFactory = nullptr;

		//깊이 연산 모드 전환 값
		bool ZEnable = true;

		//Fill
		bool bFill = true;

		//Cull
		bool bCull = true;

		//Normal Line Draw
		bool bNormal = false;

		//폰트 해제
		void FontRelease();

		////////////////////////////////////
		// 
		//RenderList
		//
		
		//Camera Item
		std::vector<RenderingData::CameraItem> CameraList;

		//PostProcess 항목 (PostProcess Material을 가진 Camera)
		std::vector<RenderingData::PostProcessItem> PostProcessList;

		//현재 장면 색상을 담고 있는 RenderTarget. GizmoRenderPass가 여기에 그리고 출력함.
		//MeshRenderPass가 Mesh RT로 초기화하고, PostProcessRenderPass가 PostProcess RT로 바꿈.
		std::string SceneRenderTarget = "MeshRenderPass-RT-BuiltIn";

		//Mesh Render Item
		std::vector<RenderingData::MeshRenderItem> OpaqueRenderList;
		std::vector<RenderingData::MeshRenderItem> MaskedRenderList;
		std::vector<RenderingData::MeshRenderItem> TranslucentRenderList;
		std::vector<RenderingData::MeshRenderItem> SkyBoxRenderList;

		//Debug Render Item
		std::vector<RenderingData::GizmoRenderItem> GizmoRenderList;

		//Normal Render Item
		std::vector<RenderingData::NormalRenderItem> NormalRenderList;

		//UI Render Item
		std::vector<RenderingData::UIContextItem> UIRenderList;

		//Light Render Item
		std::vector<RenderingData::LightRenderItem> LightRenderList;

		//SkyLight Render Item
		std::vector<RenderingData::FPSkyLightRenderItem> FPSkyLightList;

		//SceneCapture Item
		std::vector<RenderingData::SceneCaptureItem> SceneCaptureList;


		//Quad2D Render
		//Quad Mesh에 지정된 InRenderTarget을 BackBuffer로 Draw 
		void Quad2DRenderPass(std::string& InRenderTarget);

		//Aspect-corrected ViewPort size (Letterbox / Pillarbox)
		void CalculateViewPortSize(float Width, float Height, float& OutTopLeftX, float& OutTopLeftY, float& OutWidth, float& OutHeight);

		//MeshRenderPass sub-pass: single RenderItem draw, shared by Opaque/Masked/Translucent
		void DrawMeshRenderItem(const RenderingData::MeshRenderItem& RenderItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager, const std::string& DepthStencilState, UINT StencilRef = 0, bool bDisablePixelShader = false);

		void PreDepthStencilPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void CustomDepthStencilPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void SkyBoxRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void OpaqueRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void MaskedRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void TranslucentRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);

		//Quad2D 모델/뷰/투영 행렬 (Quad2DRenderPass와 PostProcessRenderPass가 공유)
		void MakeQuad2DMVP(RenderingData::MVPConstBuffer& MVPCB, float DisplayWidth, float DisplayHeight);

		//SceneCaptureRenderPass sub-pass: single RenderItem draw
		void DrawSceneCaptureMeshItem(const RenderingData::MeshRenderItem& RenderItem, const RenderingData::SceneCaptureItem& CaptureItem, FPAssetManager* AssetManager, const std::string& DepthStencilState);

	public:
		Renderer(RenderingDevice& Device);

		//Render List 생성 함수
		void CreateCamList();
		void CreateMeshRenderList();
		void CreateGizmoRenderList();
		void CreateUIRenderList();
		void CreateLightRenderList();
		void CreateSkyLightRenderList();
		void CreateNormalRenderList();
		void CreateSceneCaptureList();
		void CreatePostProcessList();

		//RenderPass 함수
		void ClearDepthStencil();

		void RenderTargetPresent();

		HRESULT InitializeRenderer(HWND hwnd);

		void PreDepthStencilRenderPass();

		void SceneCaptureRenderPass();

		void MeshRenderPass();

		void PostProcessRenderPass();

		void GizmoRenderPass();

		void NormalRenderPass();

		void UIRenderPass();

		HRESULT Finalize();

		RenderingDevice& GetRenderingDevice();

		//RenderTarget 재생성 (임시)
		void ResizeRenderTarget();

		//렌더링 속성 변경
		void SetZEnable(bool State) { ZEnable = State; };
		void SetbFill(bool State) { bFill = State; };
		void SetbCull(bool State) { bCull = State; };
		void SetbNormal(bool State) { bNormal = State; };
};