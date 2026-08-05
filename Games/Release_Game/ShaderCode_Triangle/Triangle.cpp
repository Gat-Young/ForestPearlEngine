#include "Triangle.h"
#include "ForestPearlEngine/FPMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include <iostream>

void Triangle::Initialize()
{
	Mesh = new FPMeshComponent(this, "Triangle/Test_Triangle.fbx");

	SetRootComponent((FPSceneComponent*)Mesh);
	Mesh->SetMeshCull(false);

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &Triangle::SetFillTriangel);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &Triangle::SetCullTriangle);

}

void Triangle::BeginPlay()
{
}

void Triangle::Tick()
{
	__super::Tick();
}

void Triangle::SetFillTriangel(FInputValue Value)
{
	isFill = !isFill;
	Mesh->SetMeshFill(isFill);
}

void Triangle::SetCullTriangle(FInputValue Value)
{
	isCull = !isCull;
	Mesh->SetMeshCull(isCull);
}
