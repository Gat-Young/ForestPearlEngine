#pragma once
#include "../Define/FPMath.h"
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
	std::vector<int>* VBIndex = nullptr;
	std::vector<int>* VertexSize = nullptr;
	bool* isFill = nullptr;
	bool* isCull = nullptr;
	FPVector3* Location = nullptr;
	FPQuaternion* Rotation = nullptr;
	FPVector3* Scale = nullptr;
	Topology* Topo = nullptr;
	void* VertexShader = nullptr;
	void* PixelShader = nullptr;
	void* VBLayout = nullptr;
};