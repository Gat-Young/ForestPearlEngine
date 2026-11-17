#include "ForestPearlEngine/GameProjectLoader.h"
#include "GameMode.h"
#include "GameController.h"
#include "AUtaTeapot.h"
#include "ADirectionalLight.h"
#include "ASkyLight.h"
#include "ASkyBox.h"

void RegistProjectName()
{
	FPPathManager::Get().Initialize("Test_SphereMap");
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadGameSetting("GameSetting.json");
}

void LoadLevel()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadLevelData("Test_SphereMap", "Test_SphereMap.json", AssetOwner::User);
}

void LoadClassRegist()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<GameMode>("GameMode");
	ClassRegistry->Register<GameController>("GameController");
	ClassRegistry->Register<AUtaTeapot>("AUtaTeapot");
	ClassRegistry->Register<ADirectionalLight>("ADirectionalLight");
	ClassRegistry->Register<ASkyLight>("ASkyLight");
	ClassRegistry->Register<ASkyBox>("ASkyBox");
}

void LoadAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//UtaTeapot Texture
	AssetLoader->LoadTexture("UtaTeapot/Default_baseColor_png.json", AssetOwner::User);

	//SphereMap Texture
	AssetLoader->LoadTexture("SphereMap/Cross_Roads_Probe_bmp.json", AssetOwner::User);

	//CubeMap Texture
	AssetLoader->LoadTexture("CubeMap/uffizi_cross_Mark_dds.json", AssetOwner::User);

	//UtaTeapot Material
	AssetLoader->LoadMaterial("UtaTeapot/Default_UtaTeapot_Up_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("UtaTeapot/default_UtaTeapot_Bottom_StaticMesh_MaterialInstance.json", AssetOwner::User);

	//UtaTeapot Static Mesh
	AssetLoader->LoadStaticMesh("UtaTeapot/UtaTeapot_Up_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("UtaTeapot/UtaTeapot_Bottom_StaticMesh.json", AssetOwner::User);
}

std::string ReturnStartLevel()
{
	return "Test_SphereMap";
}
