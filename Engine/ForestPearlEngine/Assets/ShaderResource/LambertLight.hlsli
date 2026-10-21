/////////////////////
//
// LambertLight Cal Func
//
#ifndef LAMBERT_LIGHT_HLSLI
#define LAMBERT_LIGHT_HLSLI

float4 LambertLight(float4 Normal, float4 Direction, float4 LightDiffuse, float4 LightAmbient, float4 MaterialDiffuse, float4 MaterialAmbient)
{
    float4 N = Normal;
    float4 L = Direction;
    
    float4 Diff = max(dot(N, L), 0) * LightDiffuse * MaterialDiffuse;
    float4 Amb = LightAmbient * MaterialAmbient;
    
    //광량의 합을 1로 재한
    return saturate(Diff + Amb);
    
    //return Diff + Amb;
}
#endif