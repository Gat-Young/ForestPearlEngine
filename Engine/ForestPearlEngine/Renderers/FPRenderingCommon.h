#pragma once
#include "../Define/FPMath.h"
#include "../Define/FPDataDefine.h"
#include <string>
#include "tchar.h"
#include <vector>

struct RenderItem
{
	int* Priority = nullptr;
	bool* Active = nullptr;
	std::vector<void*>* VB = nullptr;
	std::vector<int>* VertexSize = nullptr;
	int* Stride;
	int* Offset;
	bool* isFill = nullptr;
	bool* isCull = nullptr;
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;
	Topology* Topo = nullptr;
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
	void* VertexConst = nullptr;
	void* PixelConst = nullptr;
};


namespace RenderingData
{
	struct MeshRenderItem
	{
		//윗단에서 처리
		int Priority;
		bool Active;

		//핸들 처리
		std::vector<void*>* VB = nullptr;
		std::vector<int>* VertexSize = nullptr;
		int Stride;
		int Offset;

		//Rendering Device 전체에서 처리
		bool isFill;
		bool isCull;

		//주요 정보
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;
		Topology Topo;

		//Material에 대한 핸들 처리
		void* VertexShader;
		void* PixelShader;
		void* VBLayout;
		void* VertexConst;
		void* PixelConst;
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
		//상수 버퍼용
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
