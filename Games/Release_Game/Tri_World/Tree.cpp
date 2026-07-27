#include "Tree.h"
#include "ForestPearlEngine/MeshComponent.h"

void Tree::Initialize()
{
	Mesh = new MeshComponent(this, "Model/Tree/Tree.fbx");

	SetRootComponent((FPSceneComponent*)Mesh);
	Mesh->SetMeshCull(false);
}

void Tree::BeginPlay()
{

}

void Tree::Tick()
{

}