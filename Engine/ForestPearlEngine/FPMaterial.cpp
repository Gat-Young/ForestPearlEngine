#include "FPMaterial.h"
#include "../ForestPearlEngine/Shader/ShaderFactory.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include <iostream>

FPMaterial::FPMaterial()
{
	// 0번은 월드-뷰-프로젝션 , 1번은 ViewPort용 상수 버퍼임, 2번은 Light용으로 추가될 것
	//Render에서 Offset을 부여하므로 0번 부터 계산, 단 Shader 코드를 짤때는 염두할 것
	FPConstantBufferRenderDataUtil::AddConstantBuffer(VertexConstantBuffer, 0, MaterialVertexConstantBuffer);
}

bool FPMaterial::HasTexture(int Slot)
{
	if (TextureList.empty()) return false;
	if (TextureList.count(Slot) <= 0)
	{
		return false;
	}
	return true;
}

void FPMaterial::SetTexture(int Slot, std::string& TextureName, FPSamplerType SamplerType)
{
	if (Slot >= 128) { std::cout << "설정할 수 있는 텍스처 최대 크기를 넘었습니다.\n"; return; }
	if (HasTexture(Slot))
	{
		TextureList[Slot].SetTexture(TextureName);
		TextureList[Slot].SetSamplerType(SamplerType);
	}
}

void FPMaterial::SetTexture(int Slot, std::string& TextureName)
{
	if (Slot >= 128) { std::cout << "설정할 수 있는 텍스처 최대 크기를 넘었습니다.\n"; return; }
	if (HasTexture(Slot))
	{
		TextureList[Slot].SetTexture(TextureName);
	}
}

void FPMaterial::SetTextureSampler(int Slot, FPSamplerType SamplerType)
{
	if (Slot >= 128) { std::cout << "설정할 수 있는 텍스처 최대 크기를 넘었습니다.\n"; return; }
	if (HasTexture(Slot))
	{
		TextureList[Slot].SetSamplerType(SamplerType);
	}
}

void FPMaterial::AddTexture(int Slot)
{
	if (HasTexture(Slot)) { std::cout << "이미 Texture가 Slot에 존재 합니다.\n"; return; }
	TextureList[Slot] = FPTexture2D{};
	TextureItmeList[Slot] = FPTextureItem{TextureList[Slot].GetTexture(), TextureList[Slot].GetSamplerType()};
}

void FPMaterial::DeleteTexture(int Slot)
{
	if (HasTexture(Slot))
	{
		TextureList.erase(Slot);
		TextureItmeList.erase(Slot);
	}
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
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, MaterialVertexConstantBuffer);
}

void FPMaterial::SetDiffuse(FPVector4& Diffuse)
{
	MaterialVertexConstantBuffer.Diffuse = Diffuse;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, MaterialVertexConstantBuffer);
}

void FPMaterial::SetAmbient(FPVector4& Ambient)
{
	MaterialVertexConstantBuffer.Ambient = Ambient;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, MaterialVertexConstantBuffer);
}

void FPMaterial::SetEmissive(FPVector4& Emissive)
{
	MaterialVertexConstantBuffer.Emissive = Emissive;
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, MaterialVertexConstantBuffer);
}

FPVector4* FPMaterial::GetBaseColor()
{
	return &MaterialVertexConstantBuffer.BaseColor;
}

FPVector4* FPMaterial::GetDiffuse()
{
	return &MaterialVertexConstantBuffer.Diffuse;
}

FPVector4* FPMaterial::GetAmbient()
{
	return &MaterialVertexConstantBuffer.Ambient;
}

FPVector4* FPMaterial::GetEmissive()
{
	return &MaterialVertexConstantBuffer.Emissive;
}

std::unordered_map<int, FPTextureItem>* FPMaterial::GetTextureList()
{
	return &TextureItmeList;
}

FPConstantBufferRenderData* FPMaterial::GetVertexConstantBufferRenderData()
{
	return &VertexConstantBuffer;
}

FPConstantBufferRenderData* FPMaterial::GetPixelConstantBufferRenderData()
{
	return &PixelConstantBuffer;
}

void FPMaterial::SetTwoSided(bool isTwoSide)
{
	TwoSided = isTwoSide;
}

bool* FPMaterial::GetTwoSided()
{
	return &TwoSided;
}

void FPMaterial::UpdateMaterial(float DeltaTime)
{
	FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, MaterialVertexConstantBuffer);
}
