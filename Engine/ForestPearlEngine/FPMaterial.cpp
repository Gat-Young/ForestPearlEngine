#include "FPMaterial.h"
#include "../ForestPearlEngine/Shader/ShaderFactory.h"
#include "../ForestPearlEngine/Utility/FPPathManager.h"

FPMaterial::FPMaterial()
{
	std::wstring WidePath = std::filesystem::path(FPPathManager::Get().GetAssetPath("Shader/Demo.fx").c_str()).wstring();
	Filename = WidePath.c_str();
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

