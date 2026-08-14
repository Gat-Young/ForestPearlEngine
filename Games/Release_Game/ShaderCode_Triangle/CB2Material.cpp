#include "CB2Material.h"


CB2Material::CB2Material()
{
	Vscb = new VSConstBuffer;
	Pscb = new PSConstBuffer;
	VertextConst = Vscb;
	PixelConst = Pscb;
}

void CB2Material::UpdateMaterial(float DeltaTime)
{
	r += (3.141592654f / 2 * DeltaTime);

	//VS 세팅
	Vscb->x = 0.5f * sinf(r);

	//PS 세팅
	Pscb->per = fabsf(sinf(r));
	Vscb->per = fabsf(sinf(r));

	DurationTime += DeltaTime;

	if (DurationTime >= 2.0f)
	{
		count++;
		count %= 3;
		DurationTime = 0.0f;
	}


	Pscb->r = Colors[count].r;
	Pscb->g = Colors[count].g;
	Pscb->b = Colors[count].b;
	Pscb->a = Colors[count].a;

	Vscb->r = Colors[count].r;
	Vscb->g = Colors[count].g;
	Vscb->b = Colors[count].b;
	Vscb->a = Colors[count].a;
}


