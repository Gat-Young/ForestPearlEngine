#include "ForestPearlEngine/GameProjectLoader.h"
#include "ForestPearlEngine/FPAssetManager.h"
#include "AWater.h"
#include "GameMode.h"
#include "WaterAssets.h"
#include "WaterDemoModels.h"
#include <objbase.h>
#include <stdexcept>

#pragma comment(lib, "ole32.lib")

namespace
{
    // Initialize before the engine loads any WIC textures, including its defaults.
    // Lifetime and balancing stay entirely in this game project on the Runner thread.
    class WaterComApartment
    {
    public:
        WaterComApartment() : Result(CoInitializeEx(nullptr, COINIT_MULTITHREADED))
        {
            if (FAILED(Result) && Result != RPC_E_CHANGED_MODE)
                throw std::runtime_error("Water: COM initialization failed for texture loading.");
        }
        ~WaterComApartment() { if (SUCCEEDED(Result)) CoUninitialize(); }
    private:
        HRESULT Result;
    };
}

// Integration: select this directory as the game project in your existing build setup.
// No changes to the engine, solution, Runner, or render passes are required by this code.
void RegistProjectName()
{
    static WaterComApartment apartment;
    FPPathManager::Get().Initialize("Water");
    FPGameSettingData settings{};
    settings.GameName = "Water - Single Bump Reflection";
    settings.Width = 1280;
    settings.Height = 800;
    settings.MSAA = 1;
    settings.AnisotropicFiltering = 1;
    static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager())->UpdateGameSettingData(settings);
}

void LoadLevel()
{
    auto* assets = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
    assets->AddGameModeData("Water", "GameMode");
    FPActorData water{};
    water.ClassName = "AWater";
    water.ActorName = "WaterDemo";
    water.Scale_x = water.Scale_y = water.Scale_Z = 1.0f;
    assets->AddLevelData("Water", &water);
    water.ClassName = "WaterDemoModels";
    water.ActorName = "ReflectionModels";
    assets->AddLevelData("Water", &water);
}

void LoadClassRegist()
{
    auto* registry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
    registry->Register<GameMode>("GameMode");
    registry->Register<FPAController>("WaterGameController");
    registry->Register<AWater>("AWater");
    registry->Register<WaterDemoModels>("WaterDemoModels");
}

void LoadAssets()
{
    auto* assetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
    assetLoader->LoadTexture("Water/WaterBump.json", AssetOwner::User);
    WaterAssets::RegisterAssets();
}

std::string ReturnStartLevel() { return "Water"; }
