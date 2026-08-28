#include "CB2Material.h"
#include <iostream>

CB2Material::CB2Material()
{
	Vscb = new VSConstBuffer;
	Pscb = new PSConstBuffer;
	VertextConst = Vscb;
	PixelConst = Pscb;
}

void CB2Material::UpdateMaterial(float DeltaTime)
{
	r += (3.141592654f / 6)  * (DeltaTime);

	//VS 세팅
	Vscb->x = 0.5f * sinf(r);

	//PS 세팅
	Pscb->per = fabsf(sinf(r));
	Vscb->per = fabsf(sinf(r));

	if (fabsf(sinf(r)) >= 0.999)
	{
		count++;
		count %= 3;
		r = 0.0f;
		NowColor = NextColor;
		NextColor = Colors[count];
	}

	Pscb->r = Colors[count].r;
	Pscb->g = Colors[count].g;
	Pscb->b = Colors[count].b;
	Pscb->a = Colors[count].a;

	Vscb->r = (fabsf(sinf(r))) * NextColor.r + (1 - fabsf(sinf(r))) * NowColor.r;
	Vscb->g = (fabsf(sinf(r))) * NextColor.g + (1 - fabsf(sinf(r))) * NowColor.g;
	Vscb->b = (fabsf(sinf(r))) * NextColor.b + (1 - fabsf(sinf(r))) * NowColor.b;
	Vscb->a = (fabsf(sinf(r))) * NextColor.a + (1 - fabsf(sinf(r))) * NowColor.a;
}


