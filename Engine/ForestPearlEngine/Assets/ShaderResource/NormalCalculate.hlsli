#ifndef NORMAL_CALCULATE_HLSLI
#define NORMAL_CALCULATE_HLSLI

float3 TransformWorldNormal(float3 LocalNormal, matrix NormalMatrix)
{
     // CPU에서 계산한 역전치 Normal Matrix 적용
    float3 WorldNormal = mul((float3x3) NormalMatrix, LocalNormal);

    // 정규화하여 길이를 1로 유지
    float LengthSq = dot(WorldNormal, WorldNormal);

    if (LengthSq <= 1e-20f)
        return float3(0.0f, 0.0f, 0.0f);

    return WorldNormal * rsqrt(LengthSq);
}
#endif