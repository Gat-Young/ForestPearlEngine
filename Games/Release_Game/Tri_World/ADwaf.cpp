#include "ADwaf.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPMaterialInterface.h"

void ADwaf::Initialize()
{
	Head = new FPStaticMeshComponent(this, "Dwarf.x_SceneRoot_Head_StaticMesh");
	Body = new FPStaticMeshComponent(this, "Dwarf.x_SceneRoot_Body_StaticMesh");
	Beard = new FPStaticMeshComponent(this, "Dwarf.x_SceneRoot_Beard_StaticMesh");
	Armor = new FPStaticMeshComponent(this, "Dwarf.x_SceneRoot_Armor_StaticMesh");
	Pack = new FPStaticMeshComponent(this, "Dwarf.x_SceneRoot_Pack_StaticMesh");
	Sword1 = new FPStaticMeshComponent(this, "Dwarf.x_SceneRoot_Sword1_StaticMesh");

	SetRootComponent((FPSceneComponent*)Body);
	Head->SetupAttachment(RootComponent);
	Beard->SetupAttachment(Head);
	Armor->SetupAttachment(RootComponent);
	Pack->SetupAttachment(RootComponent);
	Sword1->SetupAttachment(RootComponent);


	FPAController* Controller = GetWorld()->GetController(0);
	Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &ADwaf::Move);
	Controller->GetInputComponent().BindMethod("IA_SetRotateWindmill", this, EKeyState::Pressed, &ADwaf::Rotate);
	Controller->GetInputComponent().BindMethod("IA_SetScaleWindmill", this, EKeyState::Pressed, &ADwaf::Scaling);
}

void ADwaf::BeginPlay()
{
	
}

void ADwaf::Tick()
{
	__super::Tick();
}

void ADwaf::Move(FInputValue Value)
{
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddLocalOffset(FPVector3{ move_x, 0.0f, move_y });
}

void ADwaf::Rotate(FInputValue Value)
{
	float mov = 90.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddLocalRotation(FPVector3{ 0.0f, move_x, 0.0f });
}

void ADwaf::Scaling(FInputValue Value)
{
	float mov = 1.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	//RootComponent->SetWorldScale3D(RootComponent->GetComponentScale() + FPVector3{ move_x, move_x, move_x });
	RootComponent->SetWorldScale3D(RootComponent->GetComponentScale() + FPVector3{ 0.0f, move_x, 0.0f });
}