#include "Orb.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include "../../Engine/ForestPearlEngine/TransformComponent.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include "GameWorld.h"

#include <iostream>

void FPOrb::Initialize()
{
	Transform = new TransformCompoenent();
	Transform->transform.position.x = 0.0f;
	Transform->transform.position.y = 0.0f;
	Transform->transform.position.z = 0.0f;

	Transform->transform.rotation.x = 0.0f;
	Transform->transform.rotation.y = 0.0f;
	Transform->transform.rotation.z = 0.0f;

	Transform->transform.scale.x = 1.0f;
	Transform->transform.scale.y = 1.0f;
	Transform->transform.scale.z = 1.0f;

	Mesh = new MeshComponent("Orb", &(Transform->transform));

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

	Transform->transform.Parent = &ParentActor->GetTransform().transform;
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
