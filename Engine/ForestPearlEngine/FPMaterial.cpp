#include "FPMaterial.h"
#include "../ForestPearlEngine/Shader/ShaderFactory.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include <iostream>

FPMaterial::FPMaterial()
{
	this->VertexShaderPath = "DefaultVertexShader.vso";
	this->PixelShaderPath = "DefaultPixelShader.pso";
	MaterialVertexConstantBuffer.BaseColor = FPVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	MaterialVertexConstantBuffer.Diffuse = FPVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	MaterialVertexConstantBuffer.Ambient = FPVector4{ 1.0f, 1.0f, 1.0f, 1.0f };

	// 0번은 월드-뷰-프로젝션 , 1번은 ViewPort용 상수 버퍼임, 2번은 Light용으로 추가될 것
	//Render에서 Offset을 부여하므로 0번 부터 계산, 단 Shader 코드를 짤때는 염두할 것
	FPConstantBufferRenderDataUtil::AddConstantBuffer(VertexConstantBuffer, 0, MaterialVertexConstantBuffer);
}

void FPMaterial::SetVertexShader(std::string VertexShaderPath)
{
	this->VertexShaderPath = VertexShaderPath;
}

void FPMaterial::SetPixelShader(std::string PixelShaderPath)
{
	this->PixelShaderPath = PixelShaderPath;
}

std::string* FPMaterial::GetVertexShader()
{
	return &(this->VertexShaderPath);
}

std::string* FPMaterial::GetPixelShader()
{
	return &(this->PixelShaderPath);
}

void FPMaterial::SetBaseColor(FPVector4& BaseColor)
{
	MaterialVertexConstantBuffer.BaseColor = BaseColor;
}

void FPMaterial::SetDiffuse(FPVector4& Diffuse)
{
	MaterialVertexConstantBuffer.Diffuse = Diffuse;
}

void FPMaterial::SetAmbient(FPVector4& Ambient)
{
	MaterialVertexConstantBuffer.Ambient = Ambient;
}

FPConstantBufferRenderData* FPMaterial::GetVertexConstantBufferRenderData()
{
	return &VertexConstantBuffer;
}

FPConstantBufferRenderData* FPMaterial::GetPixelConstantBufferRenderData()
{
	return &PixelConstantBuffer;
}

void FPMaterial::UpdateMaterial(float DeltaTime)
{
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 2, MaterialVertexConstantBuffer);
}
