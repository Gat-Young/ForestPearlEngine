#pragma once
#include "FPStreamableRenderAssetr.h"
#include <string>

enum FPSamplerType;
enum FPTextureType;

class FPTexture : public FPStreamableRenderAsset
{
	protected:
		std::string TextureName;
		FPSamplerType SamplerType;
		FPTextureType TextureType;

	public:
		void SetTexture(std::string& TextureName) { this->TextureName = TextureName; };
		void SetSamplerType(FPSamplerType& SamplerType) { this->SamplerType = SamplerType; };
		void SetTextureType(FPTextureType& TextureType) { this->TextureType = TextureType; };

		std::string* GetTexture() { return &TextureName; }
		FPSamplerType* GetSamplerType() { return &SamplerType; }
		FPTextureType* GetTextureType() { return &TextureType; }
};