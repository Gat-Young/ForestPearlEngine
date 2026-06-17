#include "GameController.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "GameWorld.h"
#include "GameMode.h"
#include "Orb.h"
#include <iostream>

void GameController::Initialize()
{
	FModifyInfo ModifyInfo = { ESwizzle::XYZ , ENegative::Positive };

	GetInputComponent().AddMappingKey("IA_AddObject", VK_LBUTTON, ModifyInfo);
	GetInputComponent().BindMethod("IA_AddObject", this, EKeyState::Down, &GameController::OnMouseLButtonDown);

	GetInputComponent().AddMappingKey("IA_SelectObject", VK_RBUTTON, ModifyInfo);
	GetInputComponent().BindMethod("IA_SelectObject", this, EKeyState::Down, &GameController::OnMouseRButtonDown);

	__super::Initialize();
}

void GameController::BeginPlay()
{
	__super::BeginPlay();
}

void GameController::Tick()
{
	__super::Tick();
}

void GameController::OnMouseLButtonDown(FInputValue InputValue)
{
	GameWorld* World = dynamic_cast<GameWorld*>(GetWorld());
	if (World == nullptr)
	{
		std::cout << "No GameWorld" << "\n";
		return;
	}

	GameMode* GM = dynamic_cast<GameMode*>(&World->GetGameMode());
	if (GM == nullptr)
	{
		std::cout << "No GameMode" << "\n";
		return;
	}

	FPVector2 SpawnPos = { InputValue.X, InputValue.Y };
	GM->AddOrb(SpawnPos);
}

void GameController::OnMouseRButtonDown(FInputValue InputValue)
{
	//std::cout << "OnMouseRButtonDown Called" << "\n";

	GameWorld* GW = dynamic_cast<GameWorld*>(GetWorld());
	if (GW == nullptr)
	{
		std::cout << "AddOrb :: No GameWorld" << "\n";
	}

	std::vector<FPActor*> GameActorList = GW->GetGameActorList();

	for (FPActor* GA : GameActorList)
	{
		FPOrb* Orb = dynamic_cast<FPOrb*>(GA);
		if (Orb == nullptr)
			continue;

		FPVector2 InPos = { InputValue.X, InputValue.Y };
		if (Orb->IsHitObject(InPos))
		{
			std::cout << "Orb Hitted!" << "\n";
		}
	}
}
