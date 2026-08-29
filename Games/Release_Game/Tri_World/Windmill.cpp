#include "Windmill.h"
#include "ForestPearlEngine/FPMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPGameTimer.h"

void Windmill::Initialize()
{
	Body = new FPMeshComponent(this, "Windmill/Windmill_Body.fbx");
	SetRootComponent((FPSceneComponent*)Body);
	Body->SetMeshCull(false);
 
	FPAController* Controller = GetWorld()->GetController(0);
	Controller->GetInputComponent().BindMethod("IA_SetMoveWindmill", this, EKeyState::Pressed, &Windmill::Move);
	Controller->GetInputComponent().BindMethod("IA_SetRotateWindmill", this, EKeyState::Pressed, &Windmill::Rotate);
	Controller->GetInputComponent().BindMethod("IA_SetScaleWindmill", this, EKeyState::Pressed, &Windmill::Scaling);
	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &Windmill::SetFillTriangel);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &Windmill::SetCullTriangle);
}

void Windmill::BeginPlay()
{
}

void Windmill::Tick()
{
	__super::Tick();
}

void Windmill::Move(FInputValue Value)
{
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddLocalOffset(FPVector3{ move_x, 0.0f, move_y });
}

void Windmill::Rotate(FInputValue Value)
{
	float mov = 90.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddLocalRotation(FPVector3{ 0.0f, move_x, 0.0f });
}

void Windmill::Scaling(FInputValue Value)
{
	float mov =1.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	RootComponent->SetWorldScale3D(RootComponent->GetComponentScale() + FPVector3{move_x, move_x, move_x});
}

void Windmill::SetFillTriangel(FInputValue Value)
{
	isFill = !isFill;
	Body->SetMeshFill(isFill);
}

void Windmill::SetCullTriangle(FInputValue Value)
{
	isCull = !isCull;
	Body->SetMeshCull(isCull);
}