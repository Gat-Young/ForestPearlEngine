#include "FPMaterial.h"
#include "../ForestPearlEngine/Shader/ShaderFactory.h"


FPMaterial::FPMaterial()
{
	ShaderFactory::GetShaderFactory().VertexShaderLoad(Filename, "VS_Main", "vs_5_0", &VertexShader, &VSCode);
	ShaderFactory::GetShaderFactory().PixelShaderLoad(Filename, "PS_Main", "ps_5_0", &PixelShader, &PSCode);
	ShaderFactory::GetShaderFactory().CreateInputLayout(VSCode, &VBLayout);
}

void* FPMaterial::GetVertexShaderPointer()
{
	return VertexShader;
}

void* FPMaterial::GetPixelShaderPointer()
{
	return PixelShader;
}

void* FPMaterial::GetVBLayoutPointer()
{
	return VBLayout;
}

