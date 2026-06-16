#include "GameController.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
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
	std::cout << "OnMouseLButtonDown :: Begin" << "\n";
}
