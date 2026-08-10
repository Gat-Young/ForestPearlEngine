#include "MoveVertexConstBufferMaterial.h"

MoveVertexConstBufferMaterial::MoveVertexConstBufferMaterial()
{
	cb = ConstBuffer{};
	VertexShader = &cb;
}

void MoveVertexConstBufferMaterial::UpdateMaterial(float DeltaTime)
{
	cb.r = 1; cb.g = 1; cb.b = 0; cb.a = 1;

	r += 3.141592654f / 2 * DeltaTime;
	cb.x = 0.5f * sinf(r);
}
