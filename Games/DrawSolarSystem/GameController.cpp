#include "GameController.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "GameWorld.h"
#include "GameMode.h"
#include <iostream>

void GameController::Initialize()
{
	FModifyInfo ModifyInfo = { ESwizzle::XYZ , ENegative::Positive };

	GetInputComponent().AddMappingKey("IA_AddObject", VK_LBUTTON, ModifyInfo);
	GetInputComponent().BindMethod("IA_AddObject", this, EKeyState::Down, &GameController::OnMouseLButtonDown);

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
