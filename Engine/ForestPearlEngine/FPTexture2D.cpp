#include "FPTexture2D.h"
#include "Renderers/FPRenderingCommon.h"

FPTexture2D::FPTexture2D()
{
	this->TextureName = "N/A";
	this->SamplerType = Sampler_CLAMP; //기본 설정은 CLAMP;
}
