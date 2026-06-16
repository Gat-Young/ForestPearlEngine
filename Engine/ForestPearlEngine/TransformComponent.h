#pragma once
#include "MeshRenderList.h"

struct Position;

struct Rotation;

struct Scale;

struct Transform;

class TransformCompoenent
{
	public:
		Transform transform;

	public:
		TransformCompoenent() = default;
		~TransformCompoenent() = default;
};
