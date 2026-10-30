#include "FPMaterial.h"
#include "../ForestPearlEngine/Shader/ShaderFactory.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include <iostream>

FPMaterial::FPMaterial()
{
	// 0번은 월드-뷰-프로젝션 , 1번은 ViewPort용 상수 버퍼임, 2번은 Light용으로 추가될 것
	//Render에서 Offset을 부여하므로 0번 부터 계산, 단 Shader 코드를 짤때는 염두할 것
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

bool FPMaterial::SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const std::vector<float>& Values)
{
    FPConstantBufferRenderData& Data = Type == FPShaderType::PS ? PixelConstantBuffer : VertexConstantBuffer;

    //Parameter를 순차 탐색 하면서 변경할 Parameter를 찾음
    for (size_t Index = 0; Index < ReflectionData.Parameters.size(); ++Index)
    {
        const FPReflectionParameterInfo& Parameter = ReflectionData.Parameters[Index];
        if (Parameter.Type != Type || Parameter.ConstantBufferName != ConstantBufferName || Parameter.ParameterName != ParameterName)
        {
            continue;
        }

        // 배열의 모든 원소 값이 Components * ArrayCount의 개수 여야함
        if (Parameter.Components == 0 || Parameter.ArrayCount == 0 || Values.size() != (Parameter.Components * Parameter.ArrayCount))
        {
            return false;
        }

        // 중간에 실패하더라도 기존 값이 일부만 바뀌지 않도록 범위를 먼저 검사
        size_t ElementOffset = 0;
        for (unsigned int Element = 0; Element < Parameter.ArrayCount; ++Element)
        {
            if (!FPConstantBufferRenderDataUtil::GetConstantBufferOffset(Parameter.BufferOffset, Parameter.Components, Parameter.ArrayCount, Parameter.ArrayStride, Data.Buffer.size(), Element, ElementOffset))
            {
                return false;
            }
        }

        const size_t ElementSize = static_cast<size_t>(Parameter.Components) * sizeof(float);
        for (unsigned int Element = 0; Element < Parameter.ArrayCount; ++Element)
        {
            FPConstantBufferRenderDataUtil::GetConstantBufferOffset(Parameter.BufferOffset, Parameter.Components, Parameter.ArrayCount, Parameter.ArrayStride, Data.Buffer.size(), Element, ElementOffset);
            std::memcpy(Data.Buffer.data() + ElementOffset, Values.data() + static_cast<size_t>(Element) * Parameter.Components, ElementSize);
        }
        return true;
    }

    return false;
}

bool FPMaterial::GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, std::vector<float>& Out) const
{
    const FPConstantBufferRenderData& Data = Type == FPShaderType::PS ? PixelConstantBuffer : VertexConstantBuffer;

    for (size_t Index = 0; Index < ReflectionData.Parameters.size(); ++Index)
    {
        const FPReflectionParameterInfo& Parameter = ReflectionData.Parameters[Index];
        if (Parameter.Type != Type || Parameter.ConstantBufferName != ConstantBufferName || Parameter.ParameterName != ParameterName)
        {
            continue;
        }

        if (Parameter.Components == 0 || Parameter.ArrayCount == 0 ||  Parameter.ArrayCount > static_cast<size_t>(-1) / Parameter.Components)
        {
            return false;
        }

        size_t ElementOffset = 0;
        for (unsigned int Element = 0; Element < Parameter.ArrayCount; ++Element)
        {
            if (!FPConstantBufferRenderDataUtil::GetConstantBufferOffset(Parameter.BufferOffset, Parameter.Components, Parameter.ArrayCount, Parameter.ArrayStride, Data.Buffer.size(), Element, ElementOffset))
            {
                return false;
            }
        }

        // 조회 결과에는 패딩을 포함하지 않고 실제 float 값만 반환
        std::vector<float> Result(static_cast<size_t>(Parameter.ArrayCount) * Parameter.Components);

        const size_t ElementSize = static_cast<size_t>(Parameter.Components) * sizeof(float);

        for (unsigned int Element = 0; Element < Parameter.ArrayCount; ++Element)
        {
            FPConstantBufferRenderDataUtil::GetConstantBufferOffset(Parameter.BufferOffset, Parameter.Components, Parameter.ArrayCount, Parameter.ArrayStride, Data.Buffer.size(), Element, ElementOffset);
            std::memcpy(Result.data() + static_cast<size_t>(Element) * Parameter.Components, Data.Buffer.data() + ElementOffset, ElementSize);
        }
        Out.swap(Result);
        return true;
    }
    return false;
}

bool FPMaterial::SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const FPVector4& Value)
{
    const std::vector<float> Values = { Value.x, Value.y, Value.z, Value.w };
    return SetParameter(Type, ConstantBufferName, ParameterName, Values);
}

bool FPMaterial::SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const FPVector3& Value)
{
    const std::vector<float> Values = { Value.x, Value.y, Value.z };
    return SetParameter(Type, ConstantBufferName, ParameterName, Values);
}

bool FPMaterial::SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const FPVector2& Value)
{
    const std::vector<float> Values = { Value.x, Value.y };
    return SetParameter(Type, ConstantBufferName, ParameterName, Values);
}

bool FPMaterial::SetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, const float& Value)
{
    const std::vector<float> Values = { Value };
    return SetParameter(Type, ConstantBufferName, ParameterName, Values);
}

bool FPMaterial::GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, FPVector4& Out) const
{
    std::vector<float> Values;
    if (!GetParameter(Type, ConstantBufferName, ParameterName, Values) || Values.size() != 4)
    {
        return false;
    }
    Out = FPVector4(Values[0], Values[1], Values[2], Values[3]);
    return true;
}

bool FPMaterial::GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, FPVector3& Out) const
{
    std::vector<float> Values;
    if (!GetParameter(Type, ConstantBufferName, ParameterName, Values) || Values.size() != 3)
    {
        return false;
    }
    Out = FPVector3(Values[0], Values[1], Values[2]);
    return true;
}

bool FPMaterial::GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, FPVector2& Out) const
{
    std::vector<float> Values;
    if (!GetParameter(Type, ConstantBufferName, ParameterName, Values) || Values.size() != 2)
    {
        return false;
    }
    Out.x = Values[0];
    Out.y = Values[1];
    return true;
}

bool FPMaterial::GetParameter(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, float& Out) const
{
    std::vector<float> Values;
    if (!GetParameter(Type, ConstantBufferName, ParameterName, Values) || Values.size() != 1)
    {
        return false;
    }
    Out = Values[0];
    return true;
}

bool FPMaterial::SetNamedTexture(FPShaderType ShaderType, const std::string& HLSLTexture, std::string AssetName, FPTextureType TextureType, unsigned int ArrayIndex)
{

    // Texture 배열은 각 요소를 'Textures[0]'처럼 구분하여 보관함
    const std::string ArrayElementName = HLSLTexture + "[" +std::to_string(ArrayIndex) + "]";

    for (size_t Index = 0; Index < ReflectionData.Resources.size(); ++Index)
    {
        FPReflectionResourceInfo& Resource = ReflectionData.Resources[Index];

        // 단일 Texture는 원래 이름, 배열은 원소별 이름으로 검색
        if (Resource.Name != ArrayElementName && !(ArrayIndex == 0 && Resource.Name == HLSLTexture))
        {
            continue;
        }

        if (Resource.TextureType != TextureType || Resource.Slot >= 128)
        {
            std::cout << "Texture 타입 또는 바인딩 슬롯이 일치하지 않음: " + HLSLTexture << "\n";
            return false;
        }

        const int Slot = static_cast<int>(Resource.Slot);
        Resource.AssetName = AssetName;
        Resource.TextureType = TextureType;
        return true;
    }
    return false;
}

bool FPMaterial::SetNamedSampler(FPShaderType ShaderType, const std::string& HLSLSampler, FPSamplerType SamplerType, unsigned int ArrayIndex)
{

    if (ShaderType != FPShaderType::PS || static_cast<int>(SamplerType) < 0 || SamplerType >= Sampler_MAX_)
    {
        std::cout << "Sampler 셰이더 단계 또는 타입이 유효하지 않음" << "\n";
        return false;
    }

    // 배열 원소 이름 생성
    const std::string ElementName = HLSLSampler + "[" + std::to_string(ArrayIndex) + "]";

    for (size_t Index = 0; Index < ReflectionData.Samplers.size(); ++Index)
    {
        FPReflectionSamplerInfo& Resource = ReflectionData.Samplers[Index];

        if (Resource.Name != ElementName && !(ArrayIndex == 0 && Resource.Name == HLSLSampler))
        {
            continue;
        }

        if (Resource.Slot >= 16)
        {
            std::cout << "Sampler 슬롯이 범위를 벗어남" << "\n";
            return false;
        }

        // Reflection 메타데이터 동기화
        Resource.SamplerType = SamplerType;

        return true;
    }


    std::cout << "Sampler를 찾지 못함: " + ElementName << "\n";
    return false;
}

bool FPMaterial::SetParameterElement(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, unsigned int ArrayIndex, const std::vector<float>& Values)
{
    FPConstantBufferRenderData& Data = Type == FPShaderType::PS ? PixelConstantBuffer : VertexConstantBuffer;

    for (size_t Index = 0; Index < ReflectionData.Parameters.size(); ++Index)
    {
        const FPReflectionParameterInfo& Parameter = ReflectionData.Parameters[Index];
        if (Parameter.Type != Type || Parameter.ConstantBufferName != ConstantBufferName || Parameter.ParameterName != ParameterName)
        {
            continue;
        }

        // 배열이 아닌 일반 변수에는 이 API를 사용하지 않음
        if (Parameter.ArrayStride == 0 || Values.size() != Parameter.Components)
        {
            return false;
        }

        size_t ElementOffset = 0;
        HRESULT hr = FPConstantBufferRenderDataUtil::GetConstantBufferOffset(Parameter.BufferOffset, Parameter.Components, Parameter.ArrayCount, Parameter.ArrayStride, Data.Buffer.size(), ArrayIndex, ElementOffset);
        if (FAILED(hr))
        {
            return false;
        }
        std::memcpy(Data.Buffer.data() + ElementOffset, Values.data(), Values.size() * sizeof(float));
        return true;
    }
    return false;
};

bool FPMaterial::GetParameterElement(FPShaderType Type, const std::string& ConstantBufferName, const std::string& ParameterName, unsigned int ArrayIndex, std::vector<float>& OutValues) const
{
    // 셰이더 단계에 맞는 상수 버퍼 선택
    const FPConstantBufferRenderData& Data = Type == FPShaderType::PS ? PixelConstantBuffer : VertexConstantBuffer;

    // Reflection에 등록된 Parameter 검색
    for (size_t Index = 0; Index < ReflectionData.Parameters.size(); ++Index)
    {
        const FPReflectionParameterInfo& Parameter = ReflectionData.Parameters[Index];

        // Parameter 식별 정보 확인
        if (Parameter.Type != Type || Parameter.ConstantBufferName != ConstantBufferName || Parameter.ParameterName != ParameterName)
        {
            continue;
        }

        // 배열 Parameter인지 확인
        if (Parameter.ArrayStride == 0 || Parameter.ArrayCount == 0 || Parameter.Components == 0)
        {
            return false;
        }

        // 배열 인덱스 검사
        if (ArrayIndex >= Parameter.ArrayCount)
        {
            return false;
        }

        // 기본 Offset 범위 검사
        if (Parameter.BufferOffset > Data.Buffer.size())
        {
            return false;
        }

        const size_t Remaining = Data.Buffer.size() - Parameter.BufferOffset;

        // 배열 요소의 Offset 범위 검사
        if (static_cast<size_t>(ArrayIndex) > Remaining / Parameter.ArrayStride)
        {
            return false;
        }

        const size_t ElementOffset = Parameter.BufferOffset + static_cast<size_t>(ArrayIndex) * Parameter.ArrayStride;

        const size_t ElementSize = static_cast<size_t>(Parameter.Components) * sizeof(float);

        // 실제 메모리 범위 검사
        if (ElementSize > Data.Buffer.size() - ElementOffset)
        {
            return false;
        }

        // 조회 결과 저장
        std::vector<float> Result(Parameter.Components);

        std::memcpy(Result.data(), Data.Buffer.data() + ElementOffset,ElementSize);

        OutValues = std::move(Result);

        return true;
    }

    // Parameter 검색 실패
    return false;
}

FPShaderReflectionData* FPMaterial::GetReflectionData()
{
	return &ReflectionData;
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
	//FPConstantBufferRenderDataUtil::UpdateConstantBuffer(VertexConstantBuffer, 0, MaterialVertexConstantBuffer);
}
