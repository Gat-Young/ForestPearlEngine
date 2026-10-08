#include "Tree.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"

void Tree::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Tree_StaticMesh");

	SetRootComponent((FPSceneComponent*)Mesh);
}

void Tree::BeginPlay()
{

}

void Tree::Tick()
{
	__super::Tick();
}
