#include "Triangle.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include "../../Engine/ForestPearlEngine/TransformComponent.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"

#include <iostream>

void Triangle::Initialize()
{
	Transform = new TransformCompoenent();
	Transform->transform.position.x = 0.0f;
	Transform->transform.position.y = 0.0f;
	Transform->transform.position.z = 0.0f;
					 
	Transform->transform.rotation.x = 0.0f;
	Transform->transform.rotation.y = 0.0f;
	Transform->transform.rotation.z = 0.0f;

	Transform->transform.scale.x = 1.0f;
	Transform->transform.scale.y = 1.0f;
	Transform->transform.scale.z = 1.0f;

	Mesh = new MeshComponent("Triangle", &(Transform->transform));

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

	Transform->transform.position.x = Value.X;
	Transform->transform.position.y = Value.Y;

	//Transform->transfrom.position.x = Value.X *1.f * 1 / GetWorld()->GetGameTimer()->DeltaTimeMS();
	//Transform->transfrom.position.y = Value.Y *1.f * 1 / GetWorld()->GetGameTimer()->DeltaTimeMS();

}