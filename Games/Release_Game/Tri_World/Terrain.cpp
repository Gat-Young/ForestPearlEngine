#include "Terrain.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPMaterialInterface.h"

void Terrain::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Terrain_StaticMesh");

	FPVector4 Color = { 0.678f, 1.000f, 0.184f, 1.0f };
	Mesh->GetMaterial().SetBaseColor(Color);

	SetRootComponent((FPSceneComponent*)Mesh);

}

void Terrain::BeginPlay()
{

}

void Terrain::Tick()
{
	__super::Tick();
}
