/////////////////////
//
// BlinPhong Cal Func
//
#ifndef BlinPhong_LIGHT_HLSLI
#define BlinPhong_LIGHT_HLSLI

float4 SpecularBlinPhongLight(float4 Normal, float4 Direction, float4 View, float4 LightSpecular, float4 MaterialSpecular, float4 MaterialRoughness)
{
    float4 N = Normal;
    float4 L = Direction;
    float4 V = View;
    
    float4 Specular = 0;
    
    if (dot(N.xyz, L.xyz) <= 0.0f)
    {
        return Specular;
    }
    
    float4 LV = L + V;
    LV.w = 0;
    
    // L과 V가 정반대인 경우 영벡터 정규화 방지
    if (dot(LV.xyz, LV.xyz) <= 0.00000001f)
    {
        return Specular;
    }
    
    float4 H = normalize(LV);
    
    float Roughness = saturate(MaterialRoughness.r);

    float Shininess = lerp(128.0f, 2.0f, Roughness);
    
    float SpecLight = pow(max(saturate(dot(N, H)), 0), Shininess);
    
    Specular = SpecLight * (LightSpecular * MaterialSpecular);
    
    return saturate(Specular);
}
#endif