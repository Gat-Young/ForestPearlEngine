#include "GameController.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"

#include "GameWorld.h"
#include "GameMode.h"
#include "Triangle.h"
#include <iostream>

void GameController::Initialize()
{
	FModifyInfo ModifyInfoW = { ESwizzle::YZX , ENegative::Positive };
	FModifyInfo ModifyInfoA = { ESwizzle::XYZ , ENegative::Negative };
	FModifyInfo ModifyInfoS = { ESwizzle::YZX , ENegative::Negative };
	FModifyInfo ModifyInfoD = { ESwizzle::XYZ , ENegative::Positive };

	FModifyInfo ModifyInfoTriger = { ESwizzle::XYZ , ENegative::Positive };

	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'W', ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'A', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'S', ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'D', ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetFillTriangel", VK_SPACE, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetCullTriangel", VK_F5, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetUITriangel", VK_F1, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_SetGrid", VK_F2, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_AddObject", VK_LBUTTON, ModifyInfoTriger);
	GetInputComponent().BindMethod("IA_AddObject", this, EKeyState::Down, &GameController::OnMouseLButtonDown);

	GetInputComponent().AddMappingKey("IA_SelectObject", VK_RBUTTON, ModifyInfoTriger);
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
	GameMode* GM = dynamic_cast<GameMode*>(GetWorld()->GetAuthGameMode());
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
		Triangle* Orb = dynamic_cast<Triangle*>(GA);
		if (Orb == nullptr)
			continue;

		FPVector2 InPos = { InputValue.X, InputValue.Y };
		if (Orb->IsHitObject(InPos))
		{
			//std::cout << "Orb Hitted!" << "\n";
		}
	}
}

