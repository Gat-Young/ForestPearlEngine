#pragma once

#include "ForestPearlEngine/FPMaterial.h"

class MoveVertexConstBufferMaterial : public FPMaterial
{
	struct ConstBuffer
	{
		float r, g, b, a;	//color
		float x, y, z, w;	//offset
	};

	ConstBuffer cb;

	float r = 0;

	public:
		MoveVertexConstBufferMaterial();

		void UpdateMaterial(float DeltaTime) override;
};