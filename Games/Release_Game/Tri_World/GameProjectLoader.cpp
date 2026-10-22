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

	//Texture Load

	//ToonLink Texture
	AssetLoader->LoadTexture("Clothes_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Equipment_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Eye_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Eyebrow0_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Eyebrow1_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Eyelid_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Hair_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth0_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth1_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth2_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth3_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth4_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth5_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth6_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth7_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth8_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth9_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Mouth10_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Pupil_png.json", AssetOwner::User);
	AssetLoader->LoadTexture("Skin_png.json", AssetOwner::User);

	//Material Load
	AssetLoader->LoadMaterial("clothes.002_ToonLink_BodyAndHat_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("equipment.002_ToonLink_Buckle_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("pupil.002_ToonLink_Eye_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("eyebrow0.002_ToonLink_EyeBrow1_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("skin.002_ToonLink_Face_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("hair.002_ToonLink_Hair_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("skin.002_ToonLink_Hand_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("eye.002_ToonLink_LeftEyeBackground_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("equipment.002_ToonLink_MasterSword_Scabbard_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("equipment.002_ToonLink_MasterSword_Handle_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("equipment.002_ToonLink_MasterSword_Blade_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("equipment.002_ToonLink_MasterSword_BackHandle_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("mouth0.002_ToonLink_Mouse1_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("eye.002_ToonLink_RightEyeBackground_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("equipment.002_ToonLink_Shield_Back_StaticMesh_MaterialInstance.json", AssetOwner::User);
	AssetLoader->LoadMaterial("equipment.002_ToonLink_Shield_Handle_StaticMesh_MaterialInstance.json", AssetOwner::User);

	//Static_Mesh Load
	AssetLoader->LoadStaticMesh("Terrain_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Tree_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Tree_Trunk_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Windmill_Body_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("Windmill_Wing_StaticMesh.json", AssetOwner::User);

	//Link Static Mesh
	AssetLoader->LoadStaticMesh("ToonLink_BodyAndHat_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_Buckle_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_Eye_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_EyeBrow1_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_Face_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_Hair_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_Hand_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_LeftEyeBackground_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_MasterSword_Scabbard_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_MasterSword_Handle_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_MasterSword_Blade_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_MasterSword_BackHandle_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_Mouse1_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_RightEyeBackground_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_Shield_Back_StaticMesh.json", AssetOwner::User);
	AssetLoader->LoadStaticMesh("ToonLink_Shield_Handle_StaticMesh.json", AssetOwner::User);
}

std::string ReturnStartLevel()
{
	//return "ToonLink";
	return "TriWorld";
}