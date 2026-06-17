#include "GameLevel.h"
#include "Orb.h"
#include "UI.h"
#include "../../Engine/ForestPearlEngine/TransformComponent.h"
#include "../../Engine/ForestPearlEngine/MeshRenderList.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"

#include <iostream>

void GameLevel::Initialize()
{
	ActorlList.push_back("UI");
	CurrentActorCount++;
	ActorlList.push_back("FPOrb"); //Parent
	CurrentActorCount++;
	ActorlList.push_back("FPOrb"); //Child
	CurrentActorCount++;
	ActorlList.push_back("FPOrb"); //Child2
	CurrentActorCount++;

	__super::Initialize();
}

void GameLevel::BeginPlay()
{
	__super::BeginPlay();

	FPOrb* Parent = dynamic_cast<FPOrb*>(GameActorList[1]);
	FPOrb* Child = dynamic_cast<FPOrb*>(GameActorList[2]);
	FPOrb* GrandChild = dynamic_cast<FPOrb*>(GameActorList[3]);
	if ((Parent == nullptr || Child == nullptr ) || GrandChild == nullptr)
	{
		std::cout << "Orb == nullptr" << "\n";
		return;
	}

	Parent->SetThisObjectIndex(1);
	Child->SetThisObjectIndex(2);
	GrandChild->SetThisObjectIndex(3);

	Transform* ParentTransform = &Parent->GetTransform().transform;
	Transform* ChildTransform = &Child->GetTransform().transform;
	Transform* GrandChildTransform = &GrandChild->GetTransform().transform;

	ParentTransform->scale.x = 0.3f;
	ParentTransform->scale.y = 0.3f;
	ParentTransform->scale.z = 0.3f;

	ChildTransform->position.x = 0.5f;
	ChildTransform->scale.x = 0.2f;
	ChildTransform->scale.y = 0.2f;
	ChildTransform->scale.z = 0.2f;

	GrandChildTransform->position.y = 0.3f;
	GrandChildTransform->scale.x = 0.15f;
	GrandChildTransform->scale.y = 0.15f;
	GrandChildTransform->scale.z = 0.15f;

	ChildTransform->Parent = ParentTransform;
	GrandChildTransform->Parent = ChildTransform;
}

void GameLevel::Tick()
{
	__super::Tick();

	FPOrb* Parent = dynamic_cast<FPOrb*>(GameActorList[1]);
	FPOrb* Child = dynamic_cast<FPOrb*>(GameActorList[2]);
	if (Parent == nullptr || Child == nullptr)
	{
		std::cout << "Parent Orb == nullptr" << "\n";
		return;
	}

	Transform* ParentTransform = &Parent->GetTransform().transform;
	ParentTransform->rotation.z += 10 * GetWorld()->GetGameTimer()->DeltaTime();

	Transform* ChildTransform = &Child->GetTransform().transform;
	ChildTransform->rotation.z += 50 * GetWorld()->GetGameTimer()->DeltaTime();
}
