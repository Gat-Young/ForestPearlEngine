#include "FPConstantBufferCommon.h"

namespace FPConstantBufferRenderDataUtil
{

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

        if (ArrayStride < ElementSize || static_cast<size_t>(ArrayIndex) >(Remaining - ElementSize) / ArrayStride)
        {
            return false;
        }

        OutOffset = BufferOffset + static_cast<size_t>(ArrayIndex) * ArrayStride;

        return true;
    }
}