#pragma once
#include "FPStreamableRenderAssetr.h"
#include <string>

enum FPTextureType;

class FPTexture : public FPStreamableRenderAsset
{
	protected:
		std::string TextureName;
		std::string SamplerType;
		FPTextureType TextureType;

	public:
		void SetTexture(std::string& TextureName) { this->TextureName = TextureName; };
		void SetSamplerType(std::string& SamplerType) { this->SamplerType = SamplerType; };
		void SetTextureType(FPTextureType& TextureType) { this->TextureType = TextureType; };

		std::string* GetTexture() { return &TextureName; }
		std::string* GetSamplerType() { return &SamplerType; }
		FPTextureType* GetTextureType() { return &TextureType; }
};