#include "Windmill.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "WindmillWing.h"
#include "TripleWindmillWing.h"

void Windmill::Initialize()
{
	Body = new FPStaticMeshComponent(this, "Windmill_Body_StaticMesh");
	SetRootComponent((FPSceneComponent*)Body);
 
	FPAController* Controller = GetWorld()->GetController(0);
	Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &Windmill::Move);
	Controller->GetInputComponent().BindMethod("IA_SetRotateWindmill", this, EKeyState::Pressed, &Windmill::Rotate);
	Controller->GetInputComponent().BindMethod("IA_SetScaleWindmill", this, EKeyState::Pressed, &Windmill::Scaling);
	Controller->GetInputComponent().BindMethod("IA_SetScaleWing", this, EKeyState::Pressed, &Windmill::SetScaleWing);
}

void Windmill::BeginPlay()
{
	OneWindmillWing = static_cast<WindmillWing*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "WindmillWing"));
	TripleWing = static_cast<TripleWindmillWing*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "TripleWindmillWing"));
}

void Windmill::Tick()
{
	__super::Tick();
}

void Windmill::SetOneWindmillWing(WindmillWing* Wing)
{
	OneWindmillWing = Wing;
}

void Windmill::SetTripleWindmillWing(TripleWindmillWing* Wing)
{
	TripleWing = Wing;
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

void Windmill::SetScaleWing(FInputValue Value)
{
	std::cout << "[Windmill] : " << "SetScaleWing" << "\n";
	if (OneWindmillWing != nullptr)
	{
		std::cout << "[Windmill] : " << "SetScaleWing : OneWing" << "\n";
		OneWindmillWing->SetScaleWing(Value);
	}

	if (TripleWing != nullptr)
	{
		std::cout << "[Windmill] : " << "SetScaleWing : TripleWing" << "\n";
		TripleWing->SetScaleWing(Value);
	}
}