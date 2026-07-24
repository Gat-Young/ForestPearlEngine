#include "Terrain.h"
#include "ForestPearlEngine/MeshComponent.h"

void Terrain::Initialize()
{
	Mesh = new MeshComponent(this, "Model/Terrain/Terrain.fbx");
	Mesh->SetTopology(TRIANGLELIST); // <- Topology를 변경할 수 있음

	SetRootComponent((FPSceneComponent*)Mesh);

	SetActorLocation({ 0.0f, -0.5f, 0.0f });
	SetActorScale3D({128.0f, 1.0f, 128.0f});
}

void Terrain::BeginPlay()
{

}

void Terrain::Tick()
{

}