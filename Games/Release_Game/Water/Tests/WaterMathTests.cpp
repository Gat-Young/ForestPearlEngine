#include "../WaterMath.h"
#include <cassert>
#include <cmath>
#include <iostream>

#ifdef NDEBUG
#error Build WaterMathTests with assertions enabled.
#endif

static void ExpectVector(DirectX::XMVECTOR a, DirectX::XMVECTOR b)
{
    assert(DirectX::XMVector4NearEqual(a, b, DirectX::XMVectorReplicate(0.0001f)));
}

int main()
{
    using namespace DirectX;
    const auto view = XMMatrixLookAtLH(XMVectorSet(4, 7, -12, 1),
        XMVectorSet(0, 1, 0, 1), XMVectorSet(0, 1, 0, 0));
    for (float level : {0.0f, 2.5f, -3.0f})
    {
        auto reflectionView = WaterMath::ReflectView(view, level);
        auto onPlane = XMVectorSet(3, level, 5, 1);
        ExpectVector(XMVector4Transform(onPlane, view), XMVector4Transform(onPlane, reflectionView));
        ExpectVector(XMVector4Transform(XMVectorSet(2, level + 4, 6, 1), reflectionView),
            XMVector4Transform(XMVectorSet(2, level - 4, 6, 1), view));
        // A reflected view reverses winding. Demo model materials must be two-sided.
        assert(XMVectorGetX(XMMatrixDeterminant(reflectionView)) < 0);
    }
    assert(std::abs(WaterMath::AdvanceFlow(0, 0.2f, 2.0f) - 0.4) < 0.00001);
    assert(std::abs(WaterMath::AdvanceFlow(0.9, 0.2f, 1.0f) - 0.1) < 0.00001);
    assert(WaterMath::AdvanceFlow(0.4, 0, 100) == 0.4);
    assert(std::abs(WaterMath::AdvanceFlow(0.1, -0.2f, 1.0f) - 0.9) < 0.00001);
    double phase = 0;
    for (int i = 0; i < 600; ++i) phase = WaterMath::AdvanceFlow(phase, 0.2f, 1.0f / 60);
    assert(std::abs(phase) < 0.00001 || std::abs(phase - 1.0) < 0.00001);
    assert(WaterMath::AdvanceFlow(0.4, 1.0f, -1.0f) == 0.4);
    std::cout << "PASS: reflected view, plane projection, winding, constant flow, wrap, pause, reverse, frame rate\n";
}
