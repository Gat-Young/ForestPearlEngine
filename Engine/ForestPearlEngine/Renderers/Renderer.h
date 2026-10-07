#pragma once
#include "RenderingDevice.h"
#include "../Shader/ShaderFactory.h"
#include <vector>
#include <queue>

namespace RenderingData
{
	struct MeshRenderItem;
	struct DebugRenderItem;
	struct CameraItem;
	struct UIContextItem;
	struct MeshRenderItemCompare;
	struct DebugRenderItemCompare;
	struct UIRenderItemCompare;
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
		bool bCull = false;

		//폰트 해제
		void FontRelease();

		////////////////////////////////////
		// 
		//RenderList
		//
		
		//Camera Item
		std::vector<RenderingData::CameraItem> CameraList;

		//Mesh Render Item
		std::priority_queue<RenderingData::MeshRenderItem, std::vector<RenderingData::MeshRenderItem>, RenderingData::MeshRenderItemCompare> MeshRenderQueue;

		//Debug Render Item
		std::priority_queue<RenderingData::DebugRenderItem, std::vector<RenderingData::DebugRenderItem>, RenderingData::DebugRenderItemCompare> DebugRenderQueue;

		//UI Render Item
		std::priority_queue<RenderingData::UIContextItem, std::vector<RenderingData::UIContextItem>, RenderingData::UIRenderItemCompare> UIRenderQueue;


	public:
		Renderer(RenderingDevice& Device);

		//Render List 생성 함수
		void CreateCamList();
		void CreateMeshRenderQueue();
		void CreateDebugRenderQueue();
		void CreateUIRenderQueue();

		//RenderPass 함수
		void ClearBackBuffer();

		void RenderTargetPresent();

		HRESULT InitializeRenderer(HWND hwnd);

		void MeshRenderPass();

		void DebugRenderPass();

		void UIRenderPass();

		HRESULT Finalize();

		RenderingDevice& GetRenderingDevice();

		//RenderTarget 재생성 (임시)
		void ResizeRenderTarget();

		//렌더링 속성 변경
		void SetZEnable(bool State) { ZEnable = State; };
		void SetbFill(bool State) { bFill = State; };
		void SetbCull(bool State) { bCull = State; };
};