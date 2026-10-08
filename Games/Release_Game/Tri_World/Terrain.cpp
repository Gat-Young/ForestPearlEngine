#include "Terrain.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"

void Terrain::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Terrain_StaticMesh");
	SetRootComponent((FPSceneComponent*)Mesh);


}

void Terrain::BeginPlay()
{

}

void Terrain::Tick()
{
	__super::Tick();
}