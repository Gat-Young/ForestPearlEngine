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
	Mesh = new MeshComponent(this, "Triangle3");
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
	angle += AngleSpeed * (GetWorld()->GetGameTimer()->DeltaTimeMS());
	RootComponent->SetRelativeRotation(FPVector3{ 0.0f, angle, 0.0f });

	__super::Tick();
}

void Triangle::Move(FInputValue Value)
{
	std::cout << "Move Begin!! [ " << Value.X << " : " << Value.Y << " ]\n";
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

