#include "ForestPearlEngine/GameProjectLoader.h"
#include "UI.h"
#include "GameMode.h"
#include "GameController.h"
#include "Player.h"
#include "Grid.h"
#include "Axis.h"
#include "Terrain.h"
#include "Tree.h"
#include "Windmill.h"
#include "WindmillWing.h"
#include "TripleWindmillWing.h"
#include "TripleWingWindmill.h"
#include "ADirectionalLight.h"

void RegistProjectName()
{
	FPPathManager::Get().Initialize("Tri_World");
	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	GameProjectSetting->SetWinWidth(960);
	GameProjectSetting->SetWinHeight(600);
	GameProjectSetting->ProjectSetting();
}

void LoadLevel()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
	AssetLoader->LoadLevelData("TriWorld", "TriWorld.json", AssetOwner::User);
	AssetLoader->LoadLevelData("ToonLink", "ToonLink.json", AssetOwner::User);
}

void LoadClassRegist()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<GameMode>("GameMode");
	ClassRegistry->Register<GameController>("GameController");
	ClassRegistry->Register<UI>("UI");
	ClassRegistry->Register<Player>("Player");
	ClassRegistry->Register<Tree>("Tree");
	ClassRegistry->Register<Grid>("Grid");
	ClassRegistry->Register<Axis>("Axis");
	ClassRegistry->Register<Terrain>("Terrain");
	ClassRegistry->Register<Windmill>("Windmill");
	ClassRegistry->Register<WindmillWing>("WindmillWing");
	ClassRegistry->Register<TripleWindmillWing>("TripleWindmillWing");
	ClassRegistry->Register<TripleWingWindmill>("TripleWingWindmill");
	ClassRegistry->Register<ADirectionalLight>("ADirectionalLight");


	//Material Class µî·Ï
}

void LoadAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//Shader Load
	AssetLoader->LoadVertexShader("TerrainVertexShader.json", AssetOwner::User);
	AssetLoader->LoadPixelShader("TerrainPixelShader.json", AssetOwner::User);
	AssetLoader->LoadPixelShader("WindmillWingBody_PixelShader.json", AssetOwner::User);
	AssetLoader->LoadPixelShader("TreePixelShader.json", AssetOwner::User);

	//Texture Load

	//ToonLink Texture
	AssetLoader->LoadTexture("ToonLink/Clothes_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Equipment_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Eye_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Eyebrow0_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Eyebrow1_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Eyelid_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Hair_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth0_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth1_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth2_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth3_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth4_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth5_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth6_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth7_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth8_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth9_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Mouth10_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Pupil_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("ToonLink/Skin_png.json", AssetOwner::User);

	//Windmill Texture
	AssetLoader->LoadTexture("Windmill/Windmill_jpg.json", AssetOwner::User);
	AssetLoader->LoadTexture("Windmill/Windmill_dds.json", AssetOwner::User);
	AssetLoader->LoadTexture("Windmill/Windmill_a_jpg.json", AssetOwner::User);

	//Tree Texture
	AssetLoader->LoadTexture("Tree/Tree01s_dds.json", AssetOwner::User);

	//Terrain Texture Load
	AssetLoader->LoadTexture("Terrain/lnd_micro16_dds.json", AssetOwner::User);
	AssetLoader->LoadTexture("Terrain/Field_micro02_dds.json", AssetOwner::User);

	//Material Load

	//ToonLink Material
	AssetLoader->LoadMaterial("ToonLink/clothes.002_ToonLink_BodyAndHat_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/equipment.002_ToonLink_Buckle_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/pupil.002_ToonLink_Eye_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/eyebrow0.002_ToonLink_EyeBrow1_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/skin.002_ToonLink_Face_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/hair.002_ToonLink_Hair_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/skin.002_ToonLink_Hand_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/eye.002_ToonLink_LeftEyeBackground_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/equipment.002_ToonLink_MasterSword_Scabbard_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/equipment.002_ToonLink_MasterSword_Handle_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/equipment.002_ToonLink_MasterSword_Blade_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/equipment.002_ToonLink_MasterSword_BackHandle_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/mouth0.002_ToonLink_Mouse1_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/eye.002_ToonLink_RightEyeBackground_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/equipment.002_ToonLink_Shield_Back_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("ToonLink/equipment.002_ToonLink_Shield_Handle_StaticMesh_MaterialInstance.json", AssetOwner::User);

	//Windmill Material
	AssetLoader->LoadMaterial("Windmill/WindmillBody_Material.json", AssetOwner::User);
	AssetLoader->LoadMaterial("Windmill/WindmillWing_Material.json", AssetOwner::User);

	//Tree Material
	AssetLoader->LoadMaterial("Tree/Tree_Material.json", AssetOwner::User);

	//Terrain Mayerial
	AssetLoader->LoadMaterial("Terrain/Terrain_Material.json", AssetOwner::User);

	/////////////////////////
	//Static_Mesh Load

	//Terrain Static Mesh
	AssetLoader->LoadStaticMesh("Terrain/Terrain_StaticMesh.json", AssetOwner::User);

	//Windmill Static Mesh
	AssetLoader->LoadStaticMesh("Windmill/Windmill_Body_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Windmill/Windmill_Wing_StaticMesh.json", AssetOwner::User);

	//Tree Static Mesh
	AssetLoader->LoadStaticMesh("Tree/Tree_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Tree/Tree_Trunk_StaticMesh.json", AssetOwner::User);

	//Link Static Mesh
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_BodyAndHat_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_Buckle_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_Eye_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_EyeBrow1_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_Face_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_Hair_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_Hand_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_LeftEyeBackground_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_MasterSword_Scabbard_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_MasterSword_Handle_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_MasterSword_Blade_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_MasterSword_BackHandle_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_Mouse1_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_RightEyeBackground_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_Shield_Back_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink/ToonLink_Shield_Handle_StaticMesh.json", AssetOwner::User);
}

std::string ReturnStartLevel()
{
	//return "ToonLink";
	return "TriWorld";
}