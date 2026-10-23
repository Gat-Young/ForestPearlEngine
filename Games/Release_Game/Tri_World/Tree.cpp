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


}

void Tree::BeginPlay()
{

}

void Tree::Tick()
{
	__super::Tick();
}
