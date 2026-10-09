#include "Cube.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPSpringArmComponent.h"
#include <iostream>

void Cube::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Cube_StaticMesh");

	//real Model
	//Mesh = new FPStaticMeshComponent(this, "ToonLink_StaticMesh");

	SetRootComponent((FPSceneComponent*)Mesh);

	//카메라 설정
	SpringArm = new FPSpringArmComponent(this);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 20.0f;
	SpringArm->SetRelativeRotation({ 30.0f, 0.0f, 0.0f });
	SpringArm->bUsePawnControlRotation = true;

	PlayerCamera = new FPCameraComponent(this);
	PlayerCamera->SetupAttachment(SpringArm);


	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveCube", this, EKeyState::Pressed, &Cube::Move);
	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &Cube::CameraMove);
	Controller->GetInputComponent().BindMethod("IA_SetScaleCube", this, EKeyState::Pressed, &Cube::ScaleUp);
	Controller->GetInputComponent().BindMethod("IA_FreeRotateCube", this, EKeyState::Down, &Cube::SetFreeRotate);
	Controller->GetInputComponent().BindMethod("IA_SetRotateCube", this, EKeyState::Pressed, &Cube::RotateCube);
	Controller->Possess(this);

}

void Cube::BeginPlay()
{

}

void Cube::Tick()
{
	if (bRotate)
	{
		float mov = 90.0f * GetWorld()->GetGameTimer()->DeltaTime();
		RootComponent->AddLocalRotation(FPVector3{ 0.0f, -mov, 0.0f });
	}

	__super::Tick();
}

void Cube::Move(FInputValue Value)
{
	//std::cout << "Actor Move [ " << GetActorLocation().x << " : " << GetActorLocation().y << " : " << GetActorLocation().z << " ]\n";
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddWorldOffset(FPVector3{ move_x, 0.0f, move_y });
}

void Cube::CameraMove(FInputValue value)
{
	FPAController* Controller = GetController();

	std::cout << "PlayerController Rotation [ " << Controller->GetActorRotation().x << " : " << Controller->GetActorRotation().y << " : " << Controller->GetActorRotation().z << " ]\n";
	std::cout << "SprtingArm Rotation [ " << SpringArm->GetComponentRotation().x << " : " << SpringArm->GetComponentRotation().y << " : " << SpringArm->GetComponentRotation().z << " ]\n";
	std::cout << "Actor Rotation [ " << GetActorRotation().x << " : " << GetActorRotation().y << " : " << GetActorRotation().z << " ]\n";
	std::cout << "Input [ " << value.X << " : " << value.Y << " ]\n";

	float mov = 30.0f;
	float move_x = value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	AddControllerYawInput(-move_x);
	AddControllerPitchInput(move_y);
}

void Cube::ScaleUp(FInputValue Value)
{
	float mov = 1.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	RootComponent->SetWorldScale3D(RootComponent->GetComponentScale() + FPVector3{ move_x, move_x, move_x });
}

void Cube::RotateCube(FInputValue value)
{
}

void Cube::SetFreeRotate(FInputValue value)
{
	std::cout << bRotate << " : SetFreeRotate" << "\n";
	bRotate = !(bRotate);
}

