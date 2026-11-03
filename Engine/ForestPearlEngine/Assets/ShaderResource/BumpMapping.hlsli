#ifndef BUMP_MAPPING_HLSLI
#define BUMP_MAPPING_HLSLI

//투입되는 Normal과 Tangent는 View 공간으로 옮길 것 Out Normal은 View 공간 Normal로 나옴
void CalculateBumpNormal(float4 texNorm, float4 T, float3 N, out float3 Normal)
{
    //노말 변환
    float3 NormalVS = normalize(N);

    //탄젠트 기저, 정규 직교화
    float3 Tangent = normalize(T.xyz - (dot(NormalVS, T.xyz) * NormalVS));
    float H = T.w < 0.0f ? -1.0f : 1.0f;
    float3 BiTangent = H * cross(NormalVS, T.xyz);
    
    float3x3 TBN = {
                        Tangent.x, BiTangent.x, NormalVS.x,
                        Tangent.y, BiTangent.y, NormalVS.y,
                        Tangent.z, BiTangent.z, NormalVS.z
                     };
    
    
    float3 NormalTS = normalize((texNorm * 2 - 1).xyz);
    
    NormalVS = normalize(mul(TBN, NormalTS));
    
    Normal = NormalVS;
}
#endif