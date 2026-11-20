#include "ForestPearlEngine/GameProjectLoader.h"
#include "GameMode.h"
#include "GameController.h"
#include "Player.h"
#include "ADirectionalLight.h"
#include "ASkyLight.h"
#include "ASkyBox.h"
#include "AWater.h"
#include "AColorBox.h"
#include "Cannon.h"
#include "CannonFireFX.h"
#include "CannonBall.h"
#include "Frigate.h"
#include <string>

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

	//KMCannon (ported, API/Input/Loader facade dropped)
	ClassRegistry->Register<Cannon>("Cannon");
	ClassRegistry->Register<CannonSocket>("CannonSocket");
	ClassRegistry->Register<CannonFireFX>("CannonFireFX");
	ClassRegistry->Register<CannonBall>("CannonBall");
	ClassRegistry->Register<Frigate>("Frigate");
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

	//KMCannon : cannon mesh (Canon_T2) + its FBX-material shader
	{
		AssetLoader->LoadPixelShader("KM/BlenderCannonPixelShader.json", AssetOwner::User);

		AssetLoader->LoadTexture("KM/Canon_T2/Canon_Low_Canon_BaseColor_png.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/Canon_T2/Canon_Low_Canon_Normal_png.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/Canon_T2/Canon_Low_Canon_Metallic_png.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/Canon_T2/Canon_Low_Canon_Roughness_png.json", AssetOwner::User);

		AssetLoader->LoadMaterial("KM/Canon_T2/Canon_Canon_1_StaticMesh_MaterialInstance.json", AssetOwner::User);

		AssetLoader->LoadStaticMesh("KM/Canon_T2/Canon_1_StaticMesh.json", AssetOwner::User);
	}

	//KMCannon : fire FX (smoke + muzzle flash)
	{
		AssetLoader->LoadVertexShader("KM/FXParticleVertexShader.json", AssetOwner::User);
		AssetLoader->LoadPixelShader("KM/FXParticlePixelShader.json", AssetOwner::User);
		AssetLoader->LoadBlendState("KM/BS_FX_ALPHA.json", AssetOwner::User);
		AssetLoader->LoadBlendState("KM/BS_FX_ADD.json", AssetOwner::User);

		AssetLoader->LoadTexture("KM/FX/FX_Smoke_dds.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/FX/FX_MuzzleFront_jpg.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/FX/FX_MuzzleSide_jpg.json", AssetOwner::User);

		AssetLoader->LoadMaterial("KM/FX/FX_Smoke_Material.json", AssetOwner::User);
		AssetLoader->LoadMaterial("KM/FX/FX_MuzzleFront_Material.json", AssetOwner::User);
		AssetLoader->LoadMaterial("KM/FX/FX_MuzzleSide_Material.json", AssetOwner::User);
	}

	//KMCannon : ship (frigate.fbx) - one mesh per node (Object_2 ~ Object_42), textures Image_0 ~ Image_63
	{
		AssetLoader->LoadPixelShader("KM/BlenderShipPixelShader.json", AssetOwner::User);

		for (int i = 0; i < Frigate::IMAGE_COUNT; ++i)
			AssetLoader->LoadTexture("KM/frigate/Image_" + std::to_string(i) + ".json", AssetOwner::User);

		static const char* FrigateMaterials[] =
		{
			"M_Ship01B_BarrelMetal_01_Object_2_StaticMesh_MaterialInstance.json",
			"M_Ship01B_BarrelMetal_02_Object_3_StaticMesh_MaterialInstance.json",
			"M_Ship01B_BarrelWood_01_Object_4_StaticMesh_MaterialInstance.json",
			"M_Ship01B_BarrelWood_02_Object_5_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Cannon_Object_6_StaticMesh_MaterialInstance.json",
			"M_Ship01B_CannonAxel_Object_7_StaticMesh_MaterialInstance.json",
			"M_Ship01B_CannonMetal_Object_8_StaticMesh_MaterialInstance.json",
			"M_Ship01B_CannonRope_Object_9_StaticMesh_MaterialInstance.json",
			"M_Ship01B_CannonRope_Holder_Object_10_StaticMesh_MaterialInstance.json",
			"M_Ship01B_CannonSides_Object_11_StaticMesh_MaterialInstance.json",
			"M_Ship01B_CannonSupport_Object_12_StaticMesh_MaterialInstance.json",
			"M_Ship01B_CannonWheels_Object_13_StaticMesh_MaterialInstance.json",
			"M_Ship01B_FigureHead.001_Object_14_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Flag_01_Object_15_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Flag_02_Object_16_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Glass_Object_17_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Lantern_Object_18_StaticMesh_MaterialInstance.json",
			"M_Ship01B_MetalRusty_Object_19_StaticMesh_MaterialInstance.json",
			"M_Ship01B_MetalRusty.002_Object_20_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Plank_01_Object_21_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Plank_02_Object_22_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Plank_03_Object_23_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Plank_04_Object_24_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Plank_05_Object_25_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Rope_01_Object_26_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Rope_02_Object_27_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Rope_02_Object_28_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Rope_02.002_Object_29_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Rope_03_Object_30_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Rope_04_Object_31_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Sail.002_Object_32_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Sail.002_Object_33_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Sail.002_Object_34_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Sail.002_Object_35_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Sail.002_Object_36_StaticMesh_MaterialInstance.json",
			"M_Ship01B_Spyglass_Object_37_StaticMesh_MaterialInstance.json",
			"M_Ship01B_WoodBrown_Object_38_StaticMesh_MaterialInstance.json",
			"M_Ship01B_WoodDark_Object_39_StaticMesh_MaterialInstance.json",
			"M_Ship01B_WoodPlain_01_Object_40_StaticMesh_MaterialInstance.json",
			"M_Ship01B_WoodPlain_02_Object_41_StaticMesh_MaterialInstance.json",
			"M_Ship01B_WoodPlain_03_Object_42_StaticMesh_MaterialInstance.json",
		};
		for (const char* Name : FrigateMaterials)
			AssetLoader->LoadMaterial(std::string("KM/frigate/") + Name, AssetOwner::User);

		for (int i = Frigate::MESH_FIRST; i <= Frigate::MESH_LAST; ++i)
			AssetLoader->LoadStaticMesh("KM/frigate/Object_" + std::to_string(i) + "_StaticMesh.json", AssetOwner::User);
	}
}

std::string ReturnStartLevel()
{
	return "DemoWaterShip";
}
