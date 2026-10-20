#include "Tree.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPMaterialInterface.h"

void Tree::Initialize()
{
	TreeLeaf = new FPStaticMeshComponent(this, "Tree_StaticMesh");
	TreeLeaf->SetupAttachment(RootComponent);
	TreeTrunk = new FPStaticMeshComponent(this, "Tree_Trunk_StaticMesh");
	TreeTrunk->SetupAttachment(RootComponent);
	
	FPVector4 Color = { 0.000f, 0.392f, 0.000f, 1.0f };
	TreeLeaf->GetMaterial().SetBaseColor(Color);
	TreeTrunk->GetMaterial().SetBaseColor(Color);

}

void Tree::BeginPlay()
{

}

void Tree::Tick()
{
	__super::Tick();
}
