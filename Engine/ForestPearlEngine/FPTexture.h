#pragma once
#include "FPStreamableRenderAssetr.h"
#include <string>

enum FPSamplerType;

class FPTexture : public FPStreamableRenderAsset
{
	protected:
		std::string TextureName;
		FPSamplerType SamplerType;

	public:
		void SetTexture(std::string& TextureName) { this->TextureName = TextureName; };
		void SetSamplerType(FPSamplerType& SamplerType) { this->SamplerType = SamplerType; };

		std::string* GetTexture() { return &TextureName; }
		FPSamplerType* GetSamplerType() { return &SamplerType; }
};