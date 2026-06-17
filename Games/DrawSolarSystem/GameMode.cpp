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
	std::cout << " Clicked Pos :: " << SpawnPos.x << ", " << SpawnPos.y << "\n";

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

	Transform* ParentTransform = &ParentActor->GetTransform().transform;
	Transform* ChildTransform = &SpawnedActor->GetTransform().transform;

	float ParentRotationTheta = fmodf(ParentTransform->rotation.z, 360) * 3.14159265358979323846f / 180.0f;
	float Direction = sqrt(pow(SpawnPos.x, 2) + pow(SpawnPos.y, 2));

	SpawnedActor->GetTransform().transform.Parent = &ParentActor->GetTransform().transform;

	std::cout << "ParentRotationDegree ::" << ParentRotationTheta << "\n";

	float SpawnPosX = SpawnPos.x * cos(ParentRotationTheta) + SpawnPos.y * sin(ParentRotationTheta);
	float SpawnPosY = -SpawnPos.x * sin(ParentRotationTheta) + SpawnPos.y * cos(ParentRotationTheta);

	std::cout << "SpawnPos ::" << SpawnPosX << ", " << SpawnPosY << "\n";

	SpawnedActor->GetTransform().transform.position.x = SpawnPosX;
	SpawnedActor->GetTransform().transform.position.y = SpawnPosY;
	SpawnedActor->GetTransform().transform.scale.x = 0.2;
	SpawnedActor->GetTransform().transform.scale.y = 0.2;
	SpawnedActor->GetTransform().transform.scale.z = 0.2;

	SpawnedActor->SetParnetObjectIndex(1);
}
