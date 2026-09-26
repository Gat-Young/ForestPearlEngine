#pragma once
#include "../Define/FPMath.h"
#include <string>
#include "tchar.h"
#include <vector>

enum Topology
{
	TRIANGLELIST,
	TRIANGLESTRIP,
	LINELIST
};

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
	bool** active;
	int* x;
	int* y;
	FPVector4* color;
	std::basic_string<TCHAR>* msg;
};
