#include "GameMode.h"
#include "../../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "GameLevel.h"
#include "GameWorld.h"
#include "../../../../ForestPearlEngine/Engine/ForestPearlEngine/FTransform.h"
#include "Triangle.h"
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
	//FPWorld* GW = GetWorld();
	//if (GW == nullptr)
	//{
	//	std::cout << "AddOrb :: No GameWorld" << "\n";
	//}

	//FPActor* SpawnedActor = GW->SpawnActor("Triangle");
	//std::vector<FPActor*> GameActorList = GW->GetGameActorList();

	//Triangle* ParentActor = dynamic_cast<Triangle*>(GameActorList[1]);
	//if (ParentActor == nullptr)
	//{
	//	std::cout << "AddOrb :: No ParentActor" << "\n";
	//}

	//FTransform ParentTransform = ParentActor->GetActorTransform();
	//FTransform ChildTransform = SpawnedActor->GetActorTransform();

	//float ParentRotationTheta = fmodf(ParentTransform.Rotation.z, 360) * 3.14159265358979323846f / 180.0f;

	//SpawnedActor->AttachToActor(ParentActor);

	////std::cout << "ParentRotationDegree ::" << ParentRotationTheta << "\n";

	//float SpawnPosX = SpawnPos.x * cos(ParentRotationTheta) + SpawnPos.y * sin(ParentRotationTheta);
	//float SpawnPosY = -SpawnPos.x * sin(ParentRotationTheta) + SpawnPos.y * cos(ParentRotationTheta);

	////std::cout << "SpawnPos ::" << SpawnPosX << ", " << SpawnPosY << "\n";

	//SpawnedActor->SetActorLocation(FPVector3{ SpawnPosX , SpawnPosY, 0.0f });
	//SpawnedActor->SetActorScale3D(FPVector3{ 0.2f, 0.2f, 0.2f });

}