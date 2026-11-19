#include "Loader_KMCannon.h"
#include "ForestPearlEngine/GameProjectLoader.h"
#include "Cannon.h"
#include "CannonFireFX.h"
#include "CannonBall.h"
#include "Frigate.h"
#include <string>

void Loader_KMCannon::RegisterClasses()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<Cannon>("Cannon");
	ClassRegistry->Register<CannonSocket>("CannonSocket");
	ClassRegistry->Register<CannonFireFX>("CannonFireFX");
	ClassRegistry->Register<CannonBall>("CannonBall");
	ClassRegistry->Register<Frigate>("Frigate");
}

//★ 모듈 에셋은 종류별 폴더 안의 KM 하위 폴더에 있음 (Assets/Texture/KM, Assets/Material/KM ...)
//  다른 프로젝트로 옮길 때 : Assets/*/KM 폴더들을 그대로 복사
void Loader_KMCannon::LoadAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//대포 메쉬 (Canon_T2) : FBX 재질 속성용 셰이더
	{
		AssetLoader->LoadPixelShader("KM/BlenderCannonPixelShader.json", AssetOwner::User);

		AssetLoader->LoadTexture("KM/Canon_T2/Canon_Low_Canon_BaseColor_png.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/Canon_T2/Canon_Low_Canon_Normal_png.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/Canon_T2/Canon_Low_Canon_Metallic_png.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/Canon_T2/Canon_Low_Canon_Roughness_png.json", AssetOwner::User);

		AssetLoader->LoadMaterial("KM/Canon_T2/Canon_Canon_1_StaticMesh_MaterialInstance.json", AssetOwner::User);

		AssetLoader->LoadStaticMesh("KM/Canon_T2/Canon_1_StaticMesh.json", AssetOwner::User);
	}

	//발사 FX : 연기 + 총구 섬광 (CannonFireFX)
	{
		AssetLoader->LoadVertexShader("KM/FXParticleVertexShader.json", AssetOwner::User);
		AssetLoader->LoadPixelShader("KM/FXParticlePixelShader.json", AssetOwner::User);
		//BlendState : 연기 = 알파, 섬광 = 더하기 (ONE + ONE)
		AssetLoader->LoadBlendState("KM/BS_FX_ALPHA.json", AssetOwner::User);
		AssetLoader->LoadBlendState("KM/BS_FX_ADD.json", AssetOwner::User);

		AssetLoader->LoadTexture("KM/FX/FX_Smoke_dds.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/FX/FX_MuzzleFront_jpg.json", AssetOwner::User);
		AssetLoader->LoadTexture("KM/FX/FX_MuzzleSide_jpg.json", AssetOwner::User);
		//Material : 판(Plane_StaticMesh) 마다 MakeMaterial 로 새로 만들어 씀
		AssetLoader->LoadMaterial("KM/FX/FX_Smoke_Material.json", AssetOwner::User);
		AssetLoader->LoadMaterial("KM/FX/FX_MuzzleFront_Material.json", AssetOwner::User);
		AssetLoader->LoadMaterial("KM/FX/FX_MuzzleSide_Material.json", AssetOwner::User);
	}

	//배 (frigate.fbx) : 노드마다 메쉬 하나 = Object_2 ~ Object_42, 텍스처 Image_0 ~ Image_63
	{
		AssetLoader->LoadPixelShader("KM/BlenderShipPixelShader.json", AssetOwner::User);	//메탈릭 · 러프니스 = 재질 값 (텍스처 없음)

		for (int i = 0; i < Frigate::IMAGE_COUNT; ++i)
			AssetLoader->LoadTexture("KM/frigate/Image_" + std::to_string(i) + ".json", AssetOwner::User);

		static const char* Materials[] =
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
		for (const char* Name : Materials)
			AssetLoader->LoadMaterial(std::string("KM/frigate/") + Name, AssetOwner::User);

		for (int i = Frigate::MESH_FIRST; i <= Frigate::MESH_LAST; ++i)
			AssetLoader->LoadStaticMesh("KM/frigate/Object_" + std::to_string(i) + "_StaticMesh.json", AssetOwner::User);
	}
}
