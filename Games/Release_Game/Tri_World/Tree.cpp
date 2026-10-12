#include "Tree.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPMaterialInterface.h"

void Tree::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Tree_StaticMesh");

	FPVector4 Color = { 0.000f, 0.392f, 0.000f, 1.0f };
	Mesh->GetMaterial().SetBaseColor(Color);

	SetRootComponent((FPSceneComponent*)Mesh);
}

void Tree::BeginPlay()
{

}

void Tree::Tick()
{
	__super::Tick();
}
