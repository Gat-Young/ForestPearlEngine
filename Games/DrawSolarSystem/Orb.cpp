#include "Orb.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include "../../Engine/ForestPearlEngine/MeshRenderList.h"
#include "GameWorld.h"

#include <iostream>

void FPOrb::Initialize()
{
	Mesh = new MeshComponent(this, "Orb");
	SetRootComponent((FPSceneComponent*)Mesh);

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;
}

void FPOrb::BeginPlay()
{
}

void FPOrb::Tick()
{
	GameWorld* GW = dynamic_cast<GameWorld*>(GetWorld());
	if (GW == nullptr)
	{
		std::cout << "AddOrb :: No GameWorld" << "\n";
	}

	int MaxActorCount = GW->GetCurrentActorCount() - 1;
	if (ParnetObjectIndex > MaxActorCount || ParnetObjectIndex == -1)
		return;

	std::vector<FPActor*> GameActorList = GW->GetGameActorList();

	FPOrb* ParentActor = dynamic_cast<FPOrb*>(GameActorList[ParnetObjectIndex]);
	if (ParentActor == nullptr)
	{
		std::cout << "AddOrb :: No ParentActor" << "\n";
	}

	AttachToActor(ParentActor);

	__super::Tick();
}

void FPOrb::SetOrbType(EOrbType Type)
{
	OrbType = Type;

	if (OrbType == EOrbType::Sun)
	{
		ObjectSize = 80.f;
		Speed = 20.f;
	}
	else if (OrbType == EOrbType::Planet)
	{
		ObjectSize = 60.f;
		Speed = 40.f;
	}
	else
	{
		ObjectSize = 30.f;
		Speed = 60.f;
	}
}

bool FPOrb::IsHitObject(FPVector2 InPos)
{
	float Distance = sqrt(pow(InPos.x - RootComponent->GetComponentLocation().x, 2) + pow(InPos.y - RootComponent->GetComponentLocation().y, 2));

	bool IsHitted = Distance <= DistanceThreshold;

	if (IsHitted)
	{
		if (bIsFill)
		{
			bIsFill = false;
			Mesh->SetMeshFill(false);
		}
		else
		{
			bIsFill = true;
			Mesh->SetMeshFill(true);
		}
	}

	//std::cout << ThisObjectIndex << " :: " << GetWorldPos().x << ", " << GetWorldPos().y << "\n";

	return Distance <= DistanceThreshold;
}
