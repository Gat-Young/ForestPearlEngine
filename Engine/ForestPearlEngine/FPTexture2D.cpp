#include "FPTexture2D.h"
#include "Renderers/FPRenderingCommon.h"

FPTexture2D::FPTexture2D()
{
	this->TextureName = "Default_Texture512";
	this->SamplerType = Sampler_CLAMP; //기본 설정은 CLAMP;
}
