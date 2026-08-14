#pragma once

#include "ForestPearlEngine/FPMaterial.h"

class MoveVertexConstBufferMaterial : public FPMaterial
{
	private:
		struct alignas(16) ConstBuffer
		{
			float r, g, b, a;	//color
			float x, y, z, w;	//offset
			float Rotate_x, Rotate_y, Rotate_z, Rotate_w; // Rotation;
			float scale;		//scale
		};

		ConstBuffer* cb;

		float r = 0;

	public:
		MoveVertexConstBufferMaterial();

		void UpdateMaterial(float DeltaTime) override;
};