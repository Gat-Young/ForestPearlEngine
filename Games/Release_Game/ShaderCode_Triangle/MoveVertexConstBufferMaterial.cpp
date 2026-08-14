#include "MoveVertexConstBufferMaterial.h"
#include <iostream>

MoveVertexConstBufferMaterial::MoveVertexConstBufferMaterial()
{
	cb = new ConstBuffer;
	VertextConst = cb;

}

void MoveVertexConstBufferMaterial::UpdateMaterial(float DeltaTime)
{
	r += (3.141592654f / 2 * DeltaTime);

	//cb->x = (0.5f * sinf(r));

	//cb->scale = 0.5f * fabs(cosf(r)) + 0.5f;

	//cb->Rotate_z = r;

	cb->r = 1; cb->g = 1; cb->b = 0; cb->a = 1;

}
