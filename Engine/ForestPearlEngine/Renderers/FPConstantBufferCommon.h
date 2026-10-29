#pragma once
#include <vector>
#include <cassert>

struct FPConstantBufferInfo
{
	unsigned int Slot;
	size_t Offset;
	size_t Size;
};

struct FPConstantBufferRenderData
{
	std::vector<uint8_t> Buffer;
	std::vector<FPConstantBufferInfo> ConstantBuffers;

};

namespace FPConstantBufferRenderDataUtil
{
	//Custom한 상수 버퍼가 있다면 생성자에서 호출
	template<typename T>
	void AddConstantBuffer(FPConstantBufferRenderData& RenderData, unsigned int Slot, const T& Data)
	{
		//memcpy 하기 안전한지 확인
		static_assert(std::is_trivially_copyable_v<T>);

		const size_t Offset = RenderData.Buffer.size();
		const size_t Size = sizeof(T);

		RenderData.Buffer.resize(Offset + Size);

		std::memcpy(RenderData.Buffer.data() + Offset, &Data, Size);

		RenderData.ConstantBuffers.push_back({ Slot, Offset, Size });
	};

	//Custom함 상수 버퍼를 업데이트 해야한다면 UpdateMaterial에서 호출
	template<typename T>
	void UpdateConstantBuffer(FPConstantBufferRenderData& RenderData, unsigned int Slot, const T& Data)
	{
		//memcpy 하기 안전한지 확인
		static_assert(std::is_trivially_copyable_v<T>);

		const size_t Offset = RenderData.ConstantBuffers[Slot].Offset;
		const size_t Size = RenderData.ConstantBuffers[Slot].Size;

		// 등록된 CB 크기와 실제 구조체 크기가 같은지 확인
		assert(Size == sizeof(T));

		// Buffer 범위 확인
		assert(Offset + Size <= RenderData.Buffer.size());

		std::memcpy(RenderData.Buffer.data() + Offset, &Data, Size);
	}

    bool GetConstantBufferOffset(const size_t& BufferOffset, const unsigned int& Components, const unsigned int& ArrayCount, const size_t& ArrayStride, size_t BufferSize, unsigned int ArrayIndex, size_t& OutOffset)
    {
        if (Components == 0 || Components > 4 || ArrayCount == 0 || ArrayIndex >= ArrayCount)
        {
            return false;
        }

        const size_t ElementSize = static_cast<size_t>(Components) * sizeof(float);
        if (BufferOffset > BufferSize)
        {
            return false;
        }

        const size_t Remaining = BufferSize - BufferOffset;
        if (ElementSize > Remaining)
        {
            return false;
        }

        if (ArrayStride == 0)
        {
            // 일반 Parameter는 첫 번째 원소만 접근 가능
            if (ArrayCount != 1 || ArrayIndex != 0)
            {
                return false;
            }

            OutOffset = BufferOffset;
            return true;
        }

        if (ArrayStride < ElementSize || static_cast<size_t>(ArrayIndex) > (Remaining - ElementSize) / ArrayStride)
        {
            return false;
        }

        OutOffset = BufferOffset + static_cast<size_t>(ArrayIndex) * ArrayStride;

        return true;
    }
}