#include "Triangle.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"

#include <iostream>

void Triangle::Initialize()
{
	Mesh = new MeshComponent("Triangle");

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod(this, EKeyState::Pressed, &Triangle::Move);
}

void Triangle::BeginPlay()
{
}

void Triangle::Tick()
{
}

void Triangle::Move(FPVector2 Value)
{
	std::cout << "Move Begin!! [ " << Value.x << " : " << Value.y << " ]\n";
}