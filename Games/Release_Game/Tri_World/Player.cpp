#include "Player.h"
#include "ForestPearlEngine/FPMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include <iostream>

void Player::Initialize()
{
	Mesh = new FPMeshComponent(this, "ToonLink/ToonLinkTriangle.fbx");

	//real Model
	//Mesh = new MeshComponent(this, "ToonLink/ToonLink.fbx");

	SetRootComponent((FPSceneComponent*)Mesh);

	Mesh->SetMeshCull(false);
	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &Player::Move);
	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &Player::SetFillTriangel);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &Player::SetCullTriangle);

}

void Player::BeginPlay()
{
}

void Player::Tick()
{
	//angle += AngleSpeed * (GetWorld()->GetGameTimer()->DeltaTimeMS());
	//RootComponent->SetRelativeRotation(FPVector3{ 0.0f, angle, 0.0f });

	__super::Tick();
}

void Player::Move(FInputValue Value)
{
	std::cout << "Actor Move [ " << GetActorLocation().x << " : " << GetActorLocation().y << " : " << GetActorLocation().z << " ]\n";
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	SetActorLocation(RootComponent->GetRelativeLocation() + FPVector3{ move_x, 0.0f, move_y });
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

