#include "GameCamera.h"
#include "ForestPearlEngine/CameraComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/GameTimer.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include <iostream>

void GameCamera::Initialize()
{
	Target = new FPSceneComponent(this);
	SetRootComponent(Target);
	SetActorLocation({ 0.0f, 0.0f, -0.0f });

	Camera = new CameraComponent(this);

	Camera->SetupAttachment(Target);
	Camera->SetRelativeLocation({ 0.0f, 20.0f, -45.0f });

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &GameCamera::Move);


}

void GameCamera::BeginPlay()
{
}

void GameCamera::Tick()
{
	__super::Tick();
}

void GameCamera::Move(FInputValue value)
{
	std::cout << "CameraMove [ " << GetActorRotation().x << " : " << GetActorRotation().y << " : " << GetActorRotation().z << " ]\n";
	float mov = 10.0f;
	float move_x = value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	FPVector3 currentRotation =
		RootComponent->GetComponentRotation();

	currentRotation.x += move_y;
	currentRotation.y += move_x;

	RootComponent->SetWorldRotation(currentRotation);
}