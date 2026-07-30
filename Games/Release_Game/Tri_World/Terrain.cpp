#include "Terrain.h"
#include "ForestPearlEngine/FPMeshComponent.h"

void Terrain::Initialize()
{
	Mesh = new FPMeshComponent(this, "Model/Terrain/Terrain.fbx");
	Mesh->SetTopology(TRIANGLELIST); // <- Topology를 변경할 수 있음

	SetRootComponent((FPSceneComponent*)Mesh);
	Mesh->SetPriority(0);

	SetActorLocation({ 0.0f, -0.5f, 0.0f });
	SetActorScale3D({128.0f, 1.0f, 128.0f});
}

void Terrain::BeginPlay()
{

}

void Terrain::Tick()
{

}