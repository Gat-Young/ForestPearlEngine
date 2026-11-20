#include "Player.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPSpringArmComponent.h"
#include <algorithm>

void Player::Initialize()
{
	//Small visible marker mesh (the body the third-person camera follows)
	BodyMesh = new FPStaticMeshComponent(this, "Cube_StaticMesh");
	SetRootComponent((FPSceneComponent*)BodyMesh);
	RootComponent->SetWorldScale3D({ 0.4f, 0.4f, 0.4f });

	//Third-person camera: SpringArm + Camera
	SpringArm = new FPSpringArmComponent(this);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 18.0f;
	SpringArm->SetRelativeLocation({ 0.0f, 1.0f, 0.0f });
	SpringArm->SetRelativeRotation({ 20.0f, 0.0f, 0.0f });
	SpringArm->bUsePawnControlRotation = true;

	PlayerCamera = new FPCameraComponent(this);
	PlayerCamera->SetupAttachment(SpringArm);

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMovePlayer", this, EKeyState::Pressed, &Player::Move);
	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &Player::CameraMove);
	Controller->GetInputComponent().BindMethod("IA_ZoomCamera", this, EKeyState::Pressed, &Player::CameraZoom);
	Controller->Possess(this);
}

void Player::BeginPlay()
{
}

void Player::Tick()
{
	__super::Tick();
}

void Player::Move(FInputValue Value)
{
	float mov = 12.0f;
	float dt = GetWorld()->GetGameTimer()->DeltaTime();

	//Move relative to where the camera is looking: W/S along the full look direction
	//(pitch included), A/D strafes along the horizontal right vector (yaw only).
	FPVector3 ControlRotation = GetController()->GetActorRotation();
	FPVector3 Forward = Rotate(FromEuler({ ControlRotation.x, ControlRotation.y, 0.0f }), FPVector3{ 0.0f, 0.0f, 1.0f });
	FPVector3 Right = Rotate(FromEuler({ 0.0f, ControlRotation.y, 0.0f }), FPVector3{ 1.0f, 0.0f, 0.0f });

	FPVector3 Offset = (Forward * Value.Y + Right * Value.X) * (mov * dt);
	RootComponent->AddWorldOffset(Offset);
}

void Player::CameraMove(FInputValue Value)
{
	float mov = 60.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	AddControllerYawInput(-move_x);
	AddControllerPitchInput(move_y);
}

void Player::CameraZoom(FInputValue Value)
{
	float mov = 20.0f;
	float mov_size = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	SpringArm->TargetArmLength = std::clamp(SpringArm->TargetArmLength + mov_size, 4.0f, 60.0f);
}
