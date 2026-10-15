#include "Plane.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPSpringArmComponent.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include <iostream>

void Plane::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Plane_StaticMesh");

	SetRootComponent((FPSceneComponent*)Mesh);

	//카메라 설정
	SpringArm = new FPSpringArmComponent(this);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 5.0f;
	SpringArm->SetRelativeRotation({ 30.0f, 0.0f, 0.0f });
	SpringArm->bUsePawnControlRotation = true;

	PlayerCamera = new FPCameraComponent(this);
	PlayerCamera->SetupAttachment(SpringArm);

	RootComponent->SetWorldScale3D({ 0.5f, 0.5f, 0.5f });


	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveCube", this, EKeyState::Pressed, &Plane::Move);
	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &Plane::CameraMove);
	Controller->GetInputComponent().BindMethod("IA_SetScaleCube", this, EKeyState::Pressed, &Plane::ScaleUp);
	Controller->GetInputComponent().BindMethod("IA_FreeRotateCube", this, EKeyState::Down, &Plane::SetFreeRotate);
	Controller->GetInputComponent().BindMethod("IA_SetRotateCube", this, EKeyState::Pressed, &Plane::RotateCube);

	if (this->GetName() == "Plane1") { Controller->Possess(this); bRotate = false; }
	else { bRotate = false;  PlayerCamera->Active = false; }

}

void Plane::BeginPlay()
{
	std::cout << this->GetName() << "\n";
	FPMaterialInterface& Material = Mesh->GetMaterial();
	std::string TextureName = "Checker_dds";
	Material.SetTexture(0, TextureName);
}

void Plane::Tick()
{
	if (bRotate)
	{
		float mov = 90.0f * GetWorld()->GetGameTimer()->DeltaTime();
		RootComponent->AddWorldRotation(FPVector3{ 0.0f, mov, 0.0f });
	}

	__super::Tick();
}

void Plane::Move(FInputValue Value)
{
	//std::cout << "Actor Move [ " << GetActorLocation().x << " : " << GetActorLocation().y << " : " << GetActorLocation().z << " ]\n";
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddWorldOffset(FPVector3{ move_x, 0.0f, move_y });
}

void Plane::CameraMove(FInputValue value)
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

void Plane::ScaleUp(FInputValue Value)
{
	float mov = 1.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	RootComponent->SetWorldScale3D(RootComponent->GetComponentScale() + FPVector3{ move_x, 0.0f, 0.0f });
}

void Plane::RotateCube(FInputValue value)
{
	float mov = 180.0f;
	float move_x = value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	RootComponent->AddWorldRotation(FPVector3{ move_y, -move_x, 0.0f });
}

void Plane::SetFreeRotate(FInputValue value)
{
	std::cout << bRotate << " : SetFreeRotate" << "\n";
	bRotate = !(bRotate);
}

