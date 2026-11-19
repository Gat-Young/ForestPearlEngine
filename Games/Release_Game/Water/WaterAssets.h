#pragma once

namespace WaterAssets
{
    // Called by GameProjectLoader::LoadAssets, before AllAssetDataCreate.
    void RegisterAssets();
    // Called after AllAssetDataCreate; requires WaterBump to have been loaded from JSON.
    void LoadShadersAndBump();
}
