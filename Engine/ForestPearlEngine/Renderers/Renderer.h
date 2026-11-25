#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
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

		//Material의 bUsePlanarForwardReflections가 true인 Mesh 항목 (위 목록들과는 상호 배타적으로 분류됨)
		std::vector<RenderingData::MeshRenderItem> PlanarReflectionOpaqueRenderList;
		std::vector<RenderingData::MeshRenderItem> PlanarReflectionMaskedRenderList;
		std::vector<RenderingData::MeshRenderItem> PlanarReflectionmTranslucentRenderList;

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

		//Planar Reflection Item
		std::vector<RenderingData::PlanarReflectionItem> PlanarReflectionList;

		//Cascade Shadow Render Item
		std::vector<RenderingData::FPCascadeShadowRenderItem> CascadeShadowList;

		//Material이 샘플링하는 RenderTarget 텍스처 이름으로 조회하는, RenderTarget별 캐싱된 ReflectionMatrix/Plane
		std::unordered_map<std::string, RenderingData::PlanarReflectionCacheData> PlanarReflectionCache;


		//Quad2D Render
		//Quad Mesh에 지정된 InRenderTarget을 BackBuffer로 Draw 
		void Quad2DRenderPass(std::string& InRenderTarget);

		//RasterizerState name resolution (bFillSolid, bCullBack) -> RS_SOLID/RS_WIREFRM/RS_CULLBACK/RS_WIRECULLBACK
		void SetRasterizerState(FPAssetManager* AssetManager, bool bFillSolid, bool bCullBack, bool bFlipCull = false);

		//Aspect-corrected ViewPort size (Letterbox / Pillarbox)
		void CalculateViewPortSize(float Width, float Height, float& OutTopLeftX, float& OutTopLeftY, float& OutWidth, float& OutHeight);

		//MeshRenderPass 서브 패스: Opaque/Masked/Translucent가 공유하는 단일 RenderItem Draw 함수
		void DrawMeshRenderItem(const RenderingData::MeshRenderItem& RenderItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager, const std::string& DepthStencilState, UINT StencilRef = 0, bool bDisablePixelShader = false, bool bFlipCull = false, bool bClipPlaneEnabled = false, const FPVector4& ClipPlane = FPVector4(), const FPMatrix& ReflectionViewProj = FPMatrix());

		//RenderItem이 샘플링하는 RenderTarget을 찾아 ReflectionMatrix * CamItem.View * (RT 화면비 보정) Projection을 반환. 없으면 Identity 반환
		FPMatrix ComputePlanarReflectionViewProj(const RenderingData::MeshRenderItem& RenderItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);

		void PreDepthStencilPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void CustomDepthStencilPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void SkyBoxRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void OpaqueRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void MaskedRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void TranslucentRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);

		//CamItem 기준으로 Planar Reflection RT를 렌더링. SceneCaptureItem도 CameraItem으로 변환해 전달 가능
		void PlanarReflectionRenderPass(const RenderingData::CameraItem& CamItem);
		void PlanarReflectionOpaqueRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void PlanarReflectionMaskedRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void PlanarReflectionmTranslucentRenderPass(const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);

		//SceneCaptureRenderPass에서 사용하는 Planar Reflection Mesh Pass (MeshRenderPass와 동일한 구조)
		void PlanarReflectionOpaqueSceneCaptureRenderPass(const RenderingData::SceneCaptureItem& CaptureItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void PlanarReflectionMaskedSceneCaptureRenderPass(const RenderingData::SceneCaptureItem& CaptureItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);
		void PlanarReflectionmTranslucentSceneCaptureRenderPass(const RenderingData::SceneCaptureItem& CaptureItem, const RenderingData::CameraItem& CamItem, FPAssetManager* AssetManager);

		//Quad2D 모델/뷰/투영 행렬 (Quad2DRenderPass와 PostProcessRenderPass가 공유)
		void MakeQuad2DMVP(RenderingData::MVPConstBuffer& MVPCB, float DisplayWidth, float DisplayHeight);

		//SceneCaptureRenderPass sub-pass: single RenderItem draw
		void DrawSceneCaptureMeshItem(const RenderingData::MeshRenderItem& RenderItem, const RenderingData::SceneCaptureItem& CaptureItem, FPAssetManager* AssetManager, const std::string& DepthStencilState, const FPMatrix& ReflectionViewProj = FPMatrix());

	public:
		Renderer(RenderingDevice& Device);

		//Render List 생성 함수
		void CreateCamList();
		void CreateMeshRenderList();
		void CreateGizmoRenderList();
		void CreateUIRenderList();
		void CreateLightRenderList();
		void CreateSkyLightRenderList();
		void CreateCascadeShadowRenderList();
		void CreateNormalRenderList();
		void CreateSceneCaptureList();
		void CreatePlanarReflectionList();
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