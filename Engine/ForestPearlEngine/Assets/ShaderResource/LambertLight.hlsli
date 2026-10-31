/////////////////////
//
// LambertLight Cal Func
//
#ifndef LAMBERT_LIGHT_HLSLI
#define LAMBERT_LIGHT_HLSLI

float4 LambertLight(float4 Normal, float4 Direction, float4 LightDiffuse,  float4 MaterialDiffuse)
{
    float4 N = float4(Normal.xyz, 0.0f);
    float4 L = float4(Direction.xyz, 0.0f);
    
    float4 Diff = pow((dot(N, L) * 0.5 + 0.5), 2) * LightDiffuse * MaterialDiffuse;
    
    //광량의 합을 1로 재한
    return saturate(Diff);
}
#endif