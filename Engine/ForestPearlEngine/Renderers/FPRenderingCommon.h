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
	std::string* MeshPath = nullptr;

	//핵심 데이터
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	std::string* VertexShaderPath = nullptr;
	std::string* PixelShaderPath = nullptr;

	//상수 버퍼용
	FPConstantBufferRenderData* VertexConstBuffer = nullptr;
	FPConstantBufferRenderData* PixelConstBuffer = nullptr;
};

struct GizmoRenderItem
{
	std::string* ClassName = nullptr;

	int* Priority = nullptr;
	bool* Active = nullptr;

	std::string* MeshPath = nullptr;

	//핵심 데이터
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;


	std::string* VertexShaderPath = nullptr;
	std::string* PixelShaderPath = nullptr;

	//상수 버퍼용
	FPConstantBufferRenderData* VertexConstBuffer = nullptr;
	FPConstantBufferRenderData* PixelConstBuffer = nullptr;
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

struct FPLightRenderItem
{
	bool* Active = nullptr;

	FPVector3* Direction = nullptr;
	float* Range = nullptr;

	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	//상수 버퍼용
	FPConstantBufferRenderData* VertexConstBuffer = nullptr;
	FPConstantBufferRenderData* PixelConstBuffer = nullptr;
};

namespace RenderingData
{
	struct MeshRenderItem
	{
		int Priority;

		std::string MeshPath;

		//주요 정보
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		std::string VertexShaderPath;
		std::string PixelShaderPath;

		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
	};

	struct GizmoRenderItem
	{
		int Priority;

		std::string MeshPath;

		//주요 정보
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		std::string VertexShaderPath;
		std::string PixelShaderPath;

		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
	};

	struct FPViewPort
	{
		float TopLeftX = 0.0f;
		float TopLeftY = 0.0f;
		float Width;
		float Height;
		float MinDepth = 0.0f;
		float MaxDepth = 1.0f;

		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
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

		std::vector<FPViewPort> ViewPort;
	};

	struct LightRenderItem
	{
		FPVector3 Direction;
		float Range;

		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		//상수 버퍼용
		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
	};

	struct MeshRenderItemCompare
	{
		bool operator() (const RenderingData::MeshRenderItem& Left, const RenderingData::MeshRenderItem& Right) const
		{
			return Left.Priority < Right.Priority;
		};
	};

	struct GizmoRenderItemCompare
	{
		bool operator() (const RenderingData::GizmoRenderItem& Left, const RenderingData::GizmoRenderItem& Right) const
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
