#pragma once
#include "ForestPearlEngine/FPMaterial.h"

class CB2Material : public FPMaterial
{
	private:
		struct  alignas(16) VSConstBuffer
		{
			float r, g, b, a;	//color
			float per;			//Percentage
			float x;			//offset
		};

		struct alignas(16) PSConstBuffer
		{
			float r, g, b, a;	//color
			float per;			//Percentage
		};

		VSConstBuffer* Vscb;

		PSConstBuffer* Pscb;

		float r;

		float DurationTime = 0.0f;
		int count = 0;

		//color
		struct Color
		{
			float r, g, b, a;
		};

		Color Colors[3] = {
			{ 1.0f, 0.0f, 0.0f, 1.0f }, //R
			{ 0.0f, 1.0f, 0.0f, 1.0f }, //G
			{ 0.0f, 0.0f, 1.0f, 1.0f }	//B
		};

		Color NowColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		Color NextColor = { 0.0f, 1.0f, 0.0f, 1.0f };

	public:
		CB2Material();

		void UpdateMaterial(float DeltaTime) override;
};