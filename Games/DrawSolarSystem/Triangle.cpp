#include "Triangle.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"

#include <iostream>

void Triangle::Initialize()
{
	Mesh = new MeshComponent("Triangle");

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_Move", this, EKeyState::Pressed, &Triangle::Move);
}

void Triangle::BeginPlay()
{
}

void Triangle::Tick()
{
}

void Triangle::Move(FInputValue Value)
{
	std::cout << "Move Begin!! [ " << Value.X << " : " << Value.Y << " ]\n";
}