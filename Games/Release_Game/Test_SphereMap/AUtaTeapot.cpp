#include "AUtaTeapot.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPSpringArmComponent.h"

void AUtaTeapot::Initialize()
{
	TeapotBottom = new FPStaticMeshComponent(this, "UtaTeapot_Bottom_StaticMesh");
	SetRootComponent((FPSceneComponent*)TeapotBottom);

	TeapotUp = new FPStaticMeshComponent(this, "UtaTeapot_Up_StaticMesh");
	TeapotUp->SetupAttachment(RootComponent);

	SpringArm = new FPSpringArmComponent(this);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 10.0f;
	SpringArm->SetRelativeRotation({ 20.0f, 0.0f, 0.0f });
	SpringArm->bUsePawnControlRotation = true;

	PlayerCamera = new FPCameraComponent(this);
	PlayerCamera->SetupAttachment(SpringArm);

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveTeapot", this, EKeyState::Pressed, &AUtaTeapot::Move);
	Controller->GetInputComponent().BindMethod("IA_SetRotateTeapot", this, EKeyState::Pressed, &AUtaTeapot::Rotate);
	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &AUtaTeapot::CameraMove);

	Controller->Possess(this);
}

void AUtaTeapot::BeginPlay()
{
}

void AUtaTeapot::Tick()
{
	__super::Tick();
}

void AUtaTeapot::Move(FInputValue Value)
{
	float mov = 5.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddWorldOffset(FPVector3{ move_x, 0.0f, move_y });
}

void AUtaTeapot::Rotate(FInputValue Value)
{
	float mov = 180.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddWorldRotation(FPVector3{ move_y, -move_x, 0.0f });
}

void AUtaTeapot::CameraMove(FInputValue Value)
{
	float mov = 60.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	AddControllerYawInput(-move_x);
	AddControllerPitchInput(move_y);
}
