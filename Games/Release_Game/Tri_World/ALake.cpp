#include "ALake.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPPlanarReflectionComponent.h"
#include "ForestPearlEngine/FPAssetManager.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPMaterialInterface.h"

void ALake::Initialize()
{
	//호수 수면 (Plane_StaticMesh는 회전 없이도 바닥처럼 평평하게 위를 향함)
	LakeMesh = new FPStaticMeshComponent(this, "Plane_StaticMesh");
	LakeMesh->SetupAttachment(RootComponent);
	//LakeMesh->SetRelativeRotation({ -90.0f, 0.0f, 0.0f });

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	LakeMesh->SetMaterial(AssetManager->MakeMaterial("Lake_Material"));

	//반사 평면/행렬 계산용 컴포넌트. Forward(로컬 +Z)가 평면의 법선이 되므로,
	//수면(위 방향, 로컬 +Y)과 일치하도록 90도 회전해서 사용 (시각적인 LakeMesh는 회전시키지 않음)
	ReflectionComponent = new FPPlanarReflectionComponent(this);
	ReflectionComponent->SetupAttachment(RootComponent);
	ReflectionComponent->SetRelativeRotation({ -90.0f, 0.0f, 0.0f });

	std::string LakeRenderTarget = "Lake_RenderTarget";
	std::string LakeDepthStencil = "Lake_DepthStencil";
	ReflectionComponent->SetRenderTarget(LakeRenderTarget);
	ReflectionComponent->SetDepthStencil(LakeDepthStencil);

	//호수 자신은 반사 대상(RenderList)에서 제외
	ReflectionComponent->AddHiddenActor(this);
}

void ALake::BeginPlay()
{

}

void ALake::Tick()
{
	__super::Tick();
}
