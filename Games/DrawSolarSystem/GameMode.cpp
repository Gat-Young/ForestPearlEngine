#include "GameMode.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "GameLevel.h"
#include "GameWorld.h"
#include "Orb.h"
#include "../../Engine/ForestPearlEngine/TransformComponent.h"
#include <iostream>

void GameMode::Initialize()
{
	ControllerList.push_back("GameController");
	__super::Initialize();
}

void GameMode::BeginPlay()
{
	__super::BeginPlay();
}

void GameMode::Tick()
{
	__super::Tick();
}

void GameMode::AddOrb(FPVector2 SpawnPos)
{
	std::cout << " SpawnPos :: " << SpawnPos.x << ", " << SpawnPos.y << "\n";

	GameWorld* GW = dynamic_cast<GameWorld*>(GetWorld());
	if (GW == nullptr)
	{
		std::cout << "AddOrb :: No GameWorld" << "\n";
	}

	int SpawnedActorIndex = GW->SpawnActor("FPOrb");
	std::vector<FPActor*> GameActorList = GW->GetGameActorList();

	FPOrb* SpawnedActor = dynamic_cast<FPOrb*>(GameActorList[SpawnedActorIndex]);
	if (SpawnedActor == nullptr)
	{
		std::cout << "AddOrb :: No SpawnedActor" << "\n";
	}

	FPOrb* ParentActor = dynamic_cast<FPOrb*>(GameActorList[1]);
	if (ParentActor == nullptr)
	{
		std::cout << "AddOrb :: No ParentActor" << "\n";
	}

	SpawnedActor->GetTransform().transform.Parent = &ParentActor->GetTransform().transform;

	SpawnedActor->GetTransform().transform.position.x = SpawnPos.x;
	SpawnedActor->GetTransform().transform.position.y = SpawnPos.y;
	SpawnedActor->GetTransform().transform.scale.x = 0.2;
	SpawnedActor->GetTransform().transform.scale.y = 0.2;
	SpawnedActor->GetTransform().transform.scale.z = 0.2;

	SpawnedActor->SetParnetObjectIndex(1);
}
