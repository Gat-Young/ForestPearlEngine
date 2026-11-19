#include "Loader_KMCannon.h"
#include "ForestPearlEngine/GameProjectLoader.h"
#include "Cannon.h"
#include "CannonFireFX.h"
#include "CannonBall.h"

void Loader_KMCannon::RegisterClasses()
{
	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());
	ClassRegistry->Register<Cannon>("Cannon");
	ClassRegistry->Register<CannonSocket>("CannonSocket");
	ClassRegistry->Register<CannonFireFX>("CannonFireFX");
	ClassRegistry->Register<CannonBall>("CannonBall");
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
}
