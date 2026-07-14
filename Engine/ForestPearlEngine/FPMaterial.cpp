#include "FPMaterial.h"
#include "../ForestPearlEngine/Shader/ShaderFactory.h"


FPMaterial::FPMaterial()
{
	ShaderFactory::GetShaderFactory().VertexShaderLoad(Filename, "VS_Main", "vs_5_0", &VertexShader, &VSCode);
	ShaderFactory::GetShaderFactory().PixelShaderLoad(Filename, "VS_Main", "vs_5_0", &PixelShader, &PSCode);
}

void* FPMaterial::GetVertexShaderPointer()
{
	return VertexShader;
}

void* FPMaterial::GetPixelShaderPointer()
{
	return PixelShader;
}

