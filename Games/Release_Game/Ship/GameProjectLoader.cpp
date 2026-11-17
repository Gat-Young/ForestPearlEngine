#include "ForestPearlEngine/GameProjectLoader.h"
#include "GameMode.h"
#include "GameController.h"
#include "AShip.h"
#include "ADirectionalLight.h"

void RegistProjectName()
{
	FPPathManager::Get().Initialize("Ship");
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadGameSetting("GameSetting.json");
}

void LoadLevel()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadLevelData("Ship", "Ship.json", AssetOwner::User);
}

void LoadClassRegist()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<GameMode>("GameMode");
	ClassRegistry->Register<GameController>("GameController");
	ClassRegistry->Register<AShip>("AShip");
	ClassRegistry->Register<ADirectionalLight>("ADirectionalLight");
}

void LoadAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//Ship Texture
	AssetLoader->LoadTexture("Ship/M_Ship01B_BarrelMetal_01_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_BarrelMetal_02_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_BarrelWood_01_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_BarrelWood_02_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_CannonAxel_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_CannonMetal_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_CannonRope_Holder_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_CannonSides_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_CannonSupport_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_CannonWheels_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Cannon_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_FigureHead.001_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Flag_01_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Flag_02_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Lantern_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_MetalRusty_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Plank_01_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Plank_02_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Plank_03_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Plank_04_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Plank_05_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Rope_01_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Rope_02_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Rope_03_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Rope_04_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Sail.002_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_Spyglass_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_WoodBrown_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_WoodDark_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_WoodPlain_01_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_WoodPlain_02_baseColor_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Ship/M_Ship01B_WoodPlain_03_baseColor_png.json", AssetOwner::User);

	//Ship Material
	AssetLoader->LoadMaterial("Ship/M_Ship01B_BarrelMetal_01_Object_2_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_BarrelMetal_02_Object_3_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_BarrelWood_01_Object_4_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_BarrelWood_02_Object_5_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_CannonAxel_Object_7_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_CannonMetal_Object_8_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_CannonRope_Holder_Object_10_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_CannonRope_Object_9_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_CannonSides_Object_11_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_CannonSupport_Object_12_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_CannonWheels_Object_13_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Cannon_Object_6_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_FigureHead.001_Object_14_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Flag_01_Object_15_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Flag_02_Object_16_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Glass_Object_17_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Lantern_Object_18_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_MetalRusty.002_Object_20_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_MetalRusty_Object_19_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Plank_01_Object_21_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Plank_02_Object_22_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Plank_03_Object_23_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Plank_04_Object_24_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Plank_05_Object_25_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Rope_01_Object_26_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Rope_02.002_Object_29_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Rope_02_Object_27_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Rope_02_Object_28_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Rope_03_Object_30_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Rope_04_Object_31_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Sail.002_Object_32_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Sail.002_Object_33_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Sail.002_Object_34_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Sail.002_Object_35_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Sail.002_Object_36_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_Spyglass_Object_37_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_WoodBrown_Object_38_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_WoodDark_Object_39_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_WoodPlain_01_Object_40_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_WoodPlain_02_Object_41_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Ship/M_Ship01B_WoodPlain_03_Object_42_StaticMesh_MaterialInstance.json", AssetOwner::User);

	//Ship Static Mesh
	AssetLoader->LoadStaticMesh("Ship/Object_2_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_3_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_4_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_5_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_6_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_7_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_8_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_9_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_10_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_11_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_12_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_13_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_14_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_15_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_16_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_17_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_18_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_19_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_20_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_21_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_22_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_23_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_24_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_25_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_26_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_27_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_28_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_29_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_30_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_31_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_32_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_33_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_34_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_35_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_36_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_37_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_38_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_39_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_40_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_41_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Ship/Object_42_StaticMesh.json", AssetOwner::User);
}

std::string ReturnStartLevel()
{
	return "Ship";
}
