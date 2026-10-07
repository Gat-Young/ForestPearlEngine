#pragma once
#include "../Define/FPMath.h"
#include "../Define/FPDataDefine.h"
#include "FPConstantBufferCommon.h"
#include <string>
#include "tchar.h"
#include <vector>



struct RenderItem
{
	int* Priority = nullptr;
	bool* Active = nullptr;
	
	//Asset에서 참조 가능
	std::vector<void*>* VB = nullptr;
	std::vector<int>* VertexSize = nullptr;
	int* Stride;
	int* Offset;

	//엔진 설정으로 변경
	bool* isFill = nullptr;
	bool* isCull = nullptr;

	//핵심 데이터
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;
	Topology* Topo = nullptr;

	//Asset에서 참조 가능
	void** VertexShader = nullptr;
	void** PixelShader = nullptr;
	void** VBLayout = nullptr;
	void** VertexConst = nullptr;
	void** PixelConst = nullptr;
};

struct CameraItem
{
	//카메라의 위치
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	FPMatrix* View = nullptr;
	FPMatrix* Projection = nullptr;

	bool* Active = nullptr;			//카메라 사용 여부
	bool* TripleCam = nullptr;
};

struct UIContextItem
{
	int* Priority;
	bool** active;
	int* x;
	int* y;
	FPVector4* color;
	std::basic_string<TCHAR>* msg;
};

struct FPViewPort
{
	float TopLeftX = 0.0f;
	float TopLeftY = 0.0f;
	float Width;
	float Height;
	float MinDepth = 0.0f;
	float MaxDepth = 1.0f;
	//상수 버퍼용
	FPConstantBufferRenderData* VertexConstBuffer = nullptr;
	FPConstantBufferRenderData* PixelConstBuffer = nullptr;
};


namespace RenderingData
{
	struct MeshRenderItem
	{
		//윗단에서 처리
		int Priority;
		bool Active;

		std::string MeshPath;
		//핸들 처리	<- AssetManager에서 값을 가져 올 수 있음
		//std::vector<void*>* VB = nullptr;
		//std::vector<int>* VertexSize = nullptr;
		//int Stride;
		//int Offset;

		//Rendering Device 전체에서 처리
		bool isFill;
		bool isCull;
		Topology Topo;

		//주요 정보
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		std::string VertexShaderPath;
		std::string PixelShaderPath;
		//Material에 대한 핸들 처리
		//void* VertexShader;			// <- AssetLoader로 처리
		//void* PixelShader;			// <- AssetLoader로 처리
		//void* VBLayout;				// <- memcpy ?
		void* VertexConst;			// <- memcpy
		void* PixelConst;			// <- memcpy
	};

	struct DebugRenderItem
	{
		int Priority;
	};

	struct FPViewPort
	{
		float TopLeftX = 0.0f;
		float TopLeftY = 0.0f;
		float Width;
		float Height;
		float MinDepth = 0.0f;
		float MaxDepth = 1.0f;

		//상수 버퍼용 <- 해당 정보도 핸들로 처리
		void* VertexConst = nullptr;
		void* PixelConst = nullptr;
	};

	struct CameraItem
	{
		//카메라의 위치
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		FPMatrix View;
		FPMatrix Projection;

		std::vector<FPViewPort> ViewPort;
	};

	struct UIContextItem
	{
		int Priority;
		bool active;
		int x;
		int y;
		FPVector4 color;
		std::basic_string<TCHAR> msg;
	};

	struct MeshRenderItemCompare
	{
		bool operator() (const RenderingData::MeshRenderItem& Left, const RenderingData::MeshRenderItem& Right) const
		{
			return Left.Priority < Right.Priority;
		};
	};

	struct DebugRenderItemCompare
	{
		bool operator() (const RenderingData::DebugRenderItem& Left, const RenderingData::DebugRenderItem& Right) const
		{
			return Left.Priority < Right.Priority;
		};
	};

	struct UIRenderItemCompare
	{
		bool operator() (const RenderingData::UIContextItem& Left, const RenderingData::UIContextItem& Right) const
		{
			return Left.Priority < Right.Priority;
		};
	};
}
