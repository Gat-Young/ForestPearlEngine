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
	Camera->SetRelativeLocation({ 0.0f, 20.0f, -45.0f });
	FPAController* Controller = GetWorld()->GetController(0);

	//if (Controller == nullptr) { return };

	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &GameCamera::Move);

	CameraRotation = FPQuaternion{ 0.0f, 0.0f, 0.0f, 1.0f };


}

void GameCamera::BeginPlay()
{
}

void GameCamera::Tick()
{
	FPActor* player = FPGameplayStatics::GetActorOfClass(GetWorld(), "Player");

	if (player != nullptr)
	{
		SetActorLocation(player->GetActorLocation());
		Camera->LookAt = player->GetActorLocation();
	}
	__super::Tick();
}

void GameCamera::Move(FInputValue value)
{
	std::cout << "CameraMove [ " << Camera->GetComponentRotation().x << " : " << Camera->GetComponentRotation().y << " : " << Camera->GetComponentRotation().z << " ]\n";
	std::cout << "CameraMove [ " << GetActorRotation().x << " : " << GetActorRotation().y << " : " << GetActorRotation().z << " ]\n";
	float mov = 30.0f;
	float move_x = value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	FPQuaternion PitchRotation = AngleAxis(DegToRad(move_y), FPVector3{ 1.0f, 0.0f, 0.0f });
	FPQuaternion YawRotation = AngleAxis(DegToRad(-move_x), FPVector3{ 0.0f, 1.0f, 0.0f });

	//로컬 x축 회전
	CameraRotation = (CameraRotation * PitchRotation).Normalize();

	//월드 y축 회전
	CameraRotation = (YawRotation * CameraRotation).Normalize();
	Camera->Up = Rotate(CameraRotation, FPVector3{ 0, 1, 0 });

	std::cout << move_x << " : " << move_y << " : " << "0.0f" << "\n";

	RootComponent->AddWorldRotation(FPVector3{ 0.0f, -move_x, 0.0f });
	RootComponent->AddLocalRotation(FPVector3{move_y, 0.0f, 0.0f});

}