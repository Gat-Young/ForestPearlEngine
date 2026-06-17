#include "Orb.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include "../../Engine/ForestPearlEngine/TransformComponent.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include "../../Engine/ForestPearlEngine/MeshRenderList.h"
#include "GameWorld.h"

#include <iostream>

void FPOrb::Initialize()
{
	FPTransform = new TransformCompoenent();
	FPTransform->transform.position.x = 0.0f;
	FPTransform->transform.position.y = 0.0f;
	FPTransform->transform.position.z = 0.0f;

	FPTransform->transform.rotation.x = 0.0f;
	FPTransform->transform.rotation.y = 0.0f;
	FPTransform->transform.rotation.z = 0.0f;

	FPTransform->transform.scale.x = 1.0f;
	FPTransform->transform.scale.y = 1.0f;
	FPTransform->transform.scale.z = 1.0f;

	Mesh = new MeshComponent("Orb", &(FPTransform->transform));

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

	FPTransform->transform.Parent = &ParentActor->GetTransform().transform;
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
	float Distance = sqrt(pow(InPos.x - GetWorldPos().x, 2) + pow(InPos.y - GetWorldPos().y, 2));

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

FPVector2 FPOrb::GetWorldPos()
{
	Transform* ParentTransform = FPTransform->transform.Parent;
	if (ParentTransform == nullptr)
		return FPVector2{0,0};

	Transform ThisTransform = FPTransform->transform;

	float ParentRotationTheta = fmodf(ParentTransform->rotation.z, 360) * 3.14159265358979323846f / 180.0f;
	//float Direction = sqrt(pow(SpawnPos.x, 2) + pow(SpawnPos.y, 2));

	////std::cout << "ParentRotationDegree ::" << ParentRotationTheta << "\n";

	float CurrentWorldX = ThisTransform.position.x * cos(ParentRotationTheta) - ThisTransform.position.y * sin(ParentRotationTheta);
	float CurrentWorldY = ThisTransform.position.x * sin(ParentRotationTheta) + ThisTransform.position.y * cos(ParentRotationTheta);

	return  FPVector2{ CurrentWorldX, CurrentWorldY };
}
