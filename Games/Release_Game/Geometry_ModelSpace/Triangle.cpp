#include "Triangle.h"
#include "ForestPearlEngine/MeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/GameTimer.h"
#include <iostream>

void Triangle::Initialize()
{
	Mesh = new MeshComponent(this, "Triangle4");
	SetRootComponent((FPSceneComponent*)Mesh);
	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &Triangle::Move);

	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &Triangle::SetFillTriangel);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &Triangle::SetCullTriangle);

}

void Triangle::BeginPlay()
{
}

void Triangle::Tick()
{
	//angle += AngleSpeed * (GetWorld()->GetGameTimer()->DeltaTimeMS());
	//RootComponent->SetRelativeRotation(FPVector3{ 0.0f, angle, 0.0f });

	__super::Tick();
}

void Triangle::Move(FInputValue Value)
{
	std::cout << "Actor Move [ " << GetActorLocation().x << " : " << GetActorLocation().y << " : " << GetActorLocation().z << " ]\n";
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	SetActorLocation(RootComponent->GetRelativeLocation() + FPVector3{ move_x, 0.0f, move_y });
}

void Triangle::SetFillTriangel(FInputValue Value)
{
	std::cout << "FillMode : " << isFill << "\n";
	isFill = !isFill;
	Mesh->SetMeshFill(isFill);
}

void Triangle::SetCullTriangle(FInputValue Value)
{
	std::cout << "CullMode : " << isCull << "\n";
	isCull = !isCull;
	Mesh->SetMeshCull(isCull);
}

