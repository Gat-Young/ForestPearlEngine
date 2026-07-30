#include "Tree.h"
#include "ForestPearlEngine/FPMeshComponent.h"

void Tree::Initialize()
{
	Mesh = new FPMeshComponent(this, "Model/Tree/Tree.fbx");

	SetRootComponent((FPSceneComponent*)Mesh);
	Mesh->SetMeshCull(false);
}

void Tree::BeginPlay()
{

}

void Tree::Tick()
{

}