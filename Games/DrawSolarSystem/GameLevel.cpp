#include "GameLevel.h"
#include "Orb.h"
#include "UI.h"
#include "../../Engine/ForestPearlEngine/MeshRenderList.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include "../../Engine/ForestPearlEngine/FTransform.h"

#include <iostream>

void GameLevel::Initialize()
{
	ActorlList.push_back("UI");
	CurrentActorCount++;
	ActorlList.push_back("FPOrb"); //Parent
	CurrentActorCount++;
	//ActorlList.push_back("FPOrb"); //Child
	//CurrentActorCount++;
	//ActorlList.push_back("FPOrb"); //Child2
	//CurrentActorCount++;
	ActorlList.push_back("GameCamera");
	CurrentActorCount++;

	__super::Initialize();
}

void GameLevel::BeginPlay()
{
	/*
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


	Parent->SetActorScale3D(FPVector3{0.3f, 0.3f, 0.3f});

	Child->SetActorLocation(FPVector3{ 0.5f, 0.0f, 0.0f });
	Child->SetActorScale3D(FPVector3{ 0.2f, 0.2f, 0.2f });

	GrandChild->SetActorLocation(FPVector3{ 0.0f, 0.3f, 0.0f });
	GrandChild->SetActorScale3D(FPVector3{ 0.15f, 0.15f, 0.15f });

	Child->AttachToActor(Parent);
	GrandChild->AttachToActor(Child);
	*/
	__super::BeginPlay();
}

void GameLevel::Tick()
{
	__super::Tick();
	/*
	FPOrb* Parent = dynamic_cast<FPOrb*>(GameActorList[1]);
	FPOrb* Child = dynamic_cast<FPOrb*>(GameActorList[2]);
	if (Parent == nullptr || Child == nullptr)
	{
		std::cout << "Parent Orb == nullptr" << "\n";
		return;
	}
	*/
	//Parent->SetActorRotation(Parent->GetActorRotation() + FPVector3{0.0f, 0.0f, 10 * GetWorld()->GetGameTimer()->DeltaTime()});

	//Child->SetActorRotation(Child->GetActorRotation() + FPVector3{ 0.0f, 0.0f, 50 * GetWorld()->GetGameTimer()->DeltaTime() });
}
