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
	Mesh->SetMeshCull(false);
}

void Tree::BeginPlay()
{

}

void Tree::Tick()
{
	__super::Tick();
}

void Tree::SetFillTriangel(FInputValue Value)
{
	isFill = !isFill;
	Mesh->SetMeshFill(isFill);
}

void Tree::SetCullTriangle(FInputValue Value)
{
	isCull = !isCull;
	Mesh->SetMeshCull(isCull);
}