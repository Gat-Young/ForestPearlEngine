#include "FPMaterial.h"
#include "../ForestPearlEngine/Shader/ShaderFactory.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include <iostream>

FPMaterial::FPMaterial()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	//Default Shader ¼¼ÆÃ
	std::pair<void*, void*> VShader = AssetManager->GetVertexShader("DefaultShader.fx");
	std::pair<void*, void*> PShader = AssetManager->GetPixelShader("DefaultShader.fx");

	VertexShader = VShader.first;
	VSCode = VShader.second;

	PixelShader = PShader.first;
	PSCode = PShader.second;

	ShaderFactory::GetShaderFactory().CreateInputLayout(VSCode, &VBLayout);
}

void FPMaterial::SetVertexShader(std::string VertexShaderPath)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	std::pair<void*, void*> VShader = AssetManager->GetVertexShader(VertexShaderPath);

	VertexShader = VShader.first;
	VSCode = VShader.second;

	ShaderFactory::GetShaderFactory().CreateInputLayout(VSCode, &VBLayout);
}

void FPMaterial::SetPixelShader(std::string PixelShaderPath)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	std::pair<void*, void*> PShader = AssetManager->GetPixelShader(PixelShaderPath);

	PixelShader = PShader.first;
	PSCode = PShader.second;
}

void** FPMaterial::GetVertexShaderPointer()
{
	return &VertexShader;
}

void** FPMaterial::GetPixelShaderPointer()
{
	return &PixelShader;
}

void** FPMaterial::GetVBLayoutPointer()
{
	return &VBLayout;
}

void** FPMaterial::GetVertexConstPointer()
{
	return &VertextConst;
}

void** FPMaterial::GetPixelConstPointer()
{
	return &PixelConst;
}

