#pragma once
#include <DirectXMath.h>
#include <cmath>

namespace WaterMath
{
    inline DirectX::XMMATRIX ReflectView(const DirectX::XMMATRIX& view, float height)
    {
        return DirectX::XMMatrixReflect(DirectX::XMVectorSet(0, 1, 0, -height)) * view;
    }

    // Keep the accumulated UV phase small without resetting when speed changes.
    inline double AdvanceFlow(double phase, float speed, float deltaTime)
    {
        if (!std::isfinite(speed) || !std::isfinite(deltaTime) || deltaTime <= 0) return phase;
        const double next = phase + static_cast<double>(speed) * deltaTime;
        return next - std::floor(next);
    }
}
