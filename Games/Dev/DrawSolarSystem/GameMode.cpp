#include "GameMode.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "GameLevel.h"
#include "GameWorld.h"
#include "../../../ForestPearlEngine/Engine/ForestPearlEngine/FTransform.h"
#include "Orb.h"
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
	//std::cout << " Clicked Pos :: " << SpawnPos.x << ", " << SpawnPos.y << "\n";

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

	SpawnedActor->SetThisObjectIndex(SpawnedActorIndex);

	FTransform ParentTransform = ParentActor->GetActorTransform();
	FTransform ChildTransform = SpawnedActor->GetActorTransform();

	float ParentRotationTheta = fmodf(ParentTransform.Rotation.z, 360) * 3.14159265358979323846f / 180.0f;

	SpawnedActor->AttachToActor(ParentActor);

	//std::cout << "ParentRotationDegree ::" << ParentRotationTheta << "\n";

	float SpawnPosX = SpawnPos.x * cos(ParentRotationTheta) + SpawnPos.y * sin(ParentRotationTheta);
	float SpawnPosY = -SpawnPos.x * sin(ParentRotationTheta) + SpawnPos.y * cos(ParentRotationTheta);

	//std::cout << "SpawnPos ::" << SpawnPosX << ", " << SpawnPosY << "\n";

	SpawnedActor->SetActorLocation(FPVector3{ SpawnPosX , SpawnPosY, 0.0f});
	SpawnedActor->SetActorScale3D(FPVector3{ 0.2f, 0.2f, 0.2f });

	SpawnedActor->SetParnetObjectIndex(1);
}
