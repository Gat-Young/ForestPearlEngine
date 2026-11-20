#include "ForestPearlEngine/GameProjectLoader.h"
#include "GameMode.h"
#include "GameController.h"
#include "Player.h"
#include "ADirectionalLight.h"
#include "ASkyLight.h"
#include "ASkyBox.h"
#include "AWater.h"
#include "AColorBox.h"

void RegistProjectName()
{
	FPPathManager::Get().Initialize("DEMO_WATER_SHIP");
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadGameSetting("GameSetting.json");
}

void LoadLevel()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadLevelData("DemoWaterShip", "DemoWaterShip.json", AssetOwner::User);
}

void LoadClassRegist()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<GameMode>("GameMode");
	ClassRegistry->Register<GameController>("GameController");
	ClassRegistry->Register<Player>("Player");
	ClassRegistry->Register<ADirectionalLight>("ADirectionalLight");
	ClassRegistry->Register<ASkyLight>("ASkyLight");
	ClassRegistry->Register<ASkyBox>("ASkyBox");
	ClassRegistry->Register<AWater>("AWater");
	ClassRegistry->Register<AColorBox>("AColorBox");
}

void LoadAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//Water Shader Load
	AssetLoader->LoadVertexShader("WaterVertexShader.json", AssetOwner::User);
	AssetLoader->LoadPixelShader("WaterPixelShader.json", AssetOwner::User);

	//Water RenderTarget / DepthStencil Load
	AssetLoader->LoadRenderTargetTexture2D("Water_RenderTarget.json", AssetOwner::User);
	AssetLoader->LoadDepthStencil("Water_DepthStencil.json", AssetOwner::User);

	//Water Texture Load
	AssetLoader->LoadTexture("Water/WaterBump.json", AssetOwner::User);

	//Water Material Load
	AssetLoader->LoadMaterial("Water/Water_Material.json", AssetOwner::User);

	//Reflection Test Box Material Load
	AssetLoader->LoadMaterial("Box/Box_Red_Material.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Box/Box_Green_Material.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Box/Box_Blue_Material.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Box/Box_Orange_Material.json", AssetOwner::User);

	//Reflection Test Box Static Mesh Load
	AssetLoader->LoadStaticMesh("ColorBox_Red_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ColorBox_Green_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ColorBox_Blue_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ColorBox_Orange_StaticMesh.json", AssetOwner::User);
}

std::string ReturnStartLevel()
{
	return "DemoWaterShip";
}
