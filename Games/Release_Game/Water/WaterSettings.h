#pragma once
#include "ForestPearlEngine/Define/FPMath.h"

// Edit these defaults, or modify AWater::Settings at runtime.
struct WaterSettings
{
    float FlowSpeed = 0.3f;           // UV units/second, along U only; zero pauses flow.
    float DistortionStrength = 0.8f;  // Height-gradient multiplier; zero is a flat mirror.
    float Tiling = 2.0f;
    float ReflectionAmount = 0.8f;
    float EnvironmentIntensity = 0.75f; // Existing SkyLight intensity; zero disables environment light.
    float EnvironmentAmount = 0.65f;   // Environment reflection contribution on the water.
    float EnvironmentDistortionScale = 6.0f; // Converts the same bump gradient into normal tilt.
    FPVector4 WaterColor = {0.035f, 0.18f, 0.24f, 1.0f};
};
