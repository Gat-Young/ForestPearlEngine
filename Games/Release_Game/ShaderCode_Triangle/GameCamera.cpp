#include "GameCamera.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include <iostream>

void GameCamera::Initialize()
{
	Camera = new FPCameraComponent(this);

	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation({ 0.0f, 0.0f, -3.0f });

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_OnTripleCam", this, EKeyState::Down, &GameCamera::OnTripleCam);

}

void GameCamera::BeginPlay()
{
}

void GameCamera::Tick()
{

	__super::Tick();
}

void GameCamera::OnTripleCam(FInputValue Value)
{
	Camera->OnTripleCam();
}