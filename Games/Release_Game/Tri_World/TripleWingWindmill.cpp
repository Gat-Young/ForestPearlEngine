#include "TripleWingWindmill.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPGameTimer.h"

void TripleWingWindmill::Initialize()
{
	Body = new FPStaticMeshComponent(this, "Windmill_Body_StaticMesh");
	SetRootComponent((FPSceneComponent*)Body);

	Wing1 = new FPStaticMeshComponent(this, "Windmill_Wing_StaticMesh");
	Wing1->SetRelativeScale3D(FPVector3(1.5f, 1.5f, 1.5f));
	Wing1->SetupAttachment(Body, "UpScaleWingPoint1");

	Wing2 = new FPStaticMeshComponent(this, "Windmill_Wing_StaticMesh");
	Wing2->SetRelativeScale3D(FPVector3(1.0f, 1.0f, 1.0f));
	Wing2->SetupAttachment(Body, "UpScaleWingPoint2");


	Wing3 = new FPStaticMeshComponent(this, "Windmill_Wing_StaticMesh");
	Wing3->SetRelativeScale3D(FPVector3(0.5f, 0.5f, 0.5f));
	Wing3->SetupAttachment(Body, "UpScaleWingPoint3");


	FPAController* Controller = GetWorld()->GetController(0);
	Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &TripleWingWindmill::Move);
	Controller->GetInputComponent().BindMethod("IA_SetRotateWindmill", this, EKeyState::Pressed, &TripleWingWindmill::Rotate);
	Controller->GetInputComponent().BindMethod("IA_SetScaleWindmill", this, EKeyState::Pressed, &TripleWingWindmill::Scaling);
}

void TripleWingWindmill::BeginPlay()
{
}

void TripleWingWindmill::Tick()
{
	FPGameTimer* GameTimer = static_cast<FPGameTimer*>(FPGameInstance::Get().GetGameTimer());

	Wing1->AddLocalRotation(FPVector3{ 0.0f,  Wing1Speed * GameTimer->DeltaTime(),0.0f });
	Wing2->AddLocalRotation(FPVector3{ 0.0f,  Wing2Speed * GameTimer->DeltaTime(),0.0f });
	Wing3->AddLocalRotation(FPVector3{ 0.0f,  - Wing3Speed * GameTimer->DeltaTime(),0.0f });
	__super::Tick();
}


void TripleWingWindmill::Move(FInputValue Value)
{
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddLocalOffset(FPVector3{ move_x, 0.0f, move_y });
}

void TripleWingWindmill::Rotate(FInputValue Value)
{
	float mov = 90.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddLocalRotation(FPVector3{ 0.0f, move_x, 0.0f });
}

void TripleWingWindmill::Scaling(FInputValue Value)
{
	float mov = 1.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->SetWorldScale3D(RootComponent->GetComponentScale() + FPVector3{ move_x, move_x, move_x });
}