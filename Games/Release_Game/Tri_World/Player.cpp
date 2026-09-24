#include "Player.h"
#include "ForestPearlEngine/FPMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPSpringArmComponent.h"
#include <iostream>

void Player::Initialize()
{
	Mesh = new FPMeshComponent(this, "ToonLink/ToonLinkTriangle.fbx");

	//real Model
	//Mesh = new MeshComponent(this, "ToonLink/ToonLink.fbx");

	SetRootComponent((FPSceneComponent*)Mesh);
	Mesh->SetMeshCull(false);

	ShieldPivot = new FPSceneComponent(this);
	ShieldPivot->SetupAttachment(Mesh);
	ShieldPivot->SetRelativeLocation({ 0.0f, 3.0f, 0.0f });

	//카메라 설정
	SpringArm = new FPSpringArmComponent(this);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 50.0f;
	SpringArm->SetRelativeRotation({ 60.0f, 0.0f, 0.0f });
	SpringArm->bUsePawnControlRotation = true;

	PlayerCamera = new FPCameraComponent(this);
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->LookAt = SpringArm->GetComponentLocation();


	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &Player::Move);
	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &Player::SetFillTriangel);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &Player::SetCullTriangle);
	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &Player::CameraMove);
	Controller->Possess(this);

}

void Player::BeginPlay()
{
}

void Player::Tick()
{
	float mov = 180.0f * GetWorld()->GetGameTimer()->DeltaTime();
	//RootComponent->AddLocalRotation(FPVector3{ 0.0f, -mov, 0.0f });

	float RotateSpeed = 360.0f;
	ShieldPivot->AddLocalRotation(FPVector3{ 0.0f,  RotateSpeed * GetWorld()->GetGameTimer()->DeltaTime(),0.0f });


	__super::Tick();
}

void Player::Move(FInputValue Value)
{
	std::cout << "Actor Move [ " << GetActorLocation().x << " : " << GetActorLocation().y << " : " << GetActorLocation().z << " ]\n";
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddWorldOffset(FPVector3{ move_x, 0.0f, move_y });
}

void Player::CameraMove(FInputValue value)
{
}

void Player::SetFillTriangel(FInputValue Value)
{
	isFill = !isFill;
	Mesh->SetMeshFill(isFill);
}

void Player::SetCullTriangle(FInputValue Value)
{
	isCull = !isCull;
	Mesh->SetMeshCull(isCull);
}

FPSceneComponent* Player::GetShieldPivot()
{
	return ShieldPivot;
}

