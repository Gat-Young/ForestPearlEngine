#include "Frigate.h"
#include "ForestPearlEngine/FPSceneComponent.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include <string>

void Frigate::Initialize()
{
	Root = new FPSceneComponent(this);
	SetRootComponent(Root);

	for (int i = MESH_FIRST; i <= MESH_LAST; ++i)
	{
		FPStaticMeshComponent* Mesh = new FPStaticMeshComponent(this, "Object_" + std::to_string(i) + "_StaticMesh");
		Mesh->SetupAttachment(Root);
		Meshes.push_back(Mesh);
	}
}

void Frigate::BeginPlay()
{
	FPActor* ShipActor = FPGameplayStatics::GetActorOfClass(GetWorld(), "Ship");
	if (ShipActor == nullptr) return;

	//레벨에 적은 월드 transform 을 유지한 채 Ship 에 붙임
	FTransform World = GetActorTransform();
	AttachToActor(ShipActor);
	SetActorLocation(World.Location);
	SetActorRotation(World.Rotation);
	SetActorScale3D(World.Scale);
}
