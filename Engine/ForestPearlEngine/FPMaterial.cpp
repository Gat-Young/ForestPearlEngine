#include "FPMaterial.h"
#include "../ForestPearlEngine/Shader/ShaderFactory.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include <iostream>

FPMaterial::FPMaterial()
{
	this->VertexShaderPath = "DefaultVertexShader.vso";
	this->PixelShaderPath = "DefaultPixelShader.pso";
}

void FPMaterial::SetVertexShader(std::string VertexShaderPath)
{
	this->VertexShaderPath = VertexShaderPath;
}

void FPMaterial::SetPixelShader(std::string PixelShaderPath)
{
	this->PixelShaderPath = PixelShaderPath;
}

FPConstantBufferRenderData* FPMaterial::GetVertexConstantBufferRenderData()
{
	return &VertexConstantBuffer;
}

FPConstantBufferRenderData* FPMaterial::GetPixelConstantBufferRenderData()
{
	return &PixelConstantBuffer;
}