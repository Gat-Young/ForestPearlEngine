#include "TripleWindmillWing.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"

void TripleWindmillWing::Initialize()
{
	Wing = new FPStaticMeshComponent(this, "Windmill_Wing_StaticMesh");
	Wing->SetMeshCull(false);
	SetRootComponent(Wing);

	Wing1 = new FPStaticMeshComponent(this, "Windmill_Wing_StaticMesh");
	Wing1->SetMeshCull(false);
	Wing1->SetupAttachment(Wing);
	Wing1->SetRelativeLocation({ 0.0f, 0.2f, 0.0f });
	Wing1->SetWorldScale3D({ 1.0f, 1.0f, 1.0f });

	Wing2 = new FPStaticMeshComponent(this, "Windmill_Wing_StaticMesh");
	Wing2->SetMeshCull(false);
	Wing2->SetupAttachment(Wing1);
	Wing2->SetRelativeLocation({ 0.0f, 0.2f, 0.0f });
	Wing2->SetWorldScale3D({ 0.5f, 0.5f, 0.5f });

	FPAController* Controller = GetWorld()->GetController(0);
	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &TripleWindmillWing::SetFillTriangel);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &TripleWindmillWing::SetCullTriangle);
	Controller->GetInputComponent().BindMethod("IA_SetScaleWing", this, EKeyState::Pressed, &TripleWindmillWing::SetScaleWing);
}

void TripleWindmillWing::BeginPlay()
{
	std::vector<FPActor*> WindmillActor;

	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "Windmill", WindmillActor);

	for (FPActor* Actor : WindmillActor)
	{
		if (Actor->GetName() == "Windmill2")
		{
			Body = Actor;
			break;
		}
	}

	AttachToActor(Body);
	Wing->SetRelativeLocation({ 0.0f, 1.0f, -1.5f });
}

void TripleWindmillWing::Tick()
{
	FPGameTimer* GameTimer = static_cast<FPGameTimer*>(FPGameInstance::Get().GetGameTimer());

	float RotateSpeed = 180.0f;
	if (isHead) RotateSpeed *= 2;
	Wing->AddLocalRotation(FPVector3{ 0.0f,  RotateSpeed * GameTimer->DeltaTime(),0.0f });
	Wing1->AddLocalRotation(FPVector3{ 0.0f,  -2 * RotateSpeed * GameTimer->DeltaTime(),0.0f });
	Wing2->AddLocalRotation(FPVector3{ 0.0f,  2 * RotateSpeed * GameTimer->DeltaTime(),0.0f });
	__super::Tick();
}

void TripleWindmillWing::SetScaleWing(FInputValue Value)
{
	float mov = 1.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	if (isShield)
	{
		ScaleOffset += move_x;

		bool isBig = ScaleOffset > 2.0f;
		bool isSmall = ScaleOffset < 0.3f;
		if (isBig) { ScaleOffset = 2.0f; return; }
		if (isSmall) { ScaleOffset = 0.3f; return; }

		Wing->AddLocalOffset({ 0.0f, 0.0f, move_x * 5 });

		std::cout << Wing->GetRelativeLocation().x << " : " << Wing->GetRelativeLocation().y << " : " << Wing->GetRelativeLocation().z << "\n";

		Wing->SetWorldScale3D(Wing->GetComponentScale() + FPVector3{ move_x, move_x, move_x });
	}
}

void TripleWindmillWing::SetFillTriangel(FInputValue Value)
{
	isFill = !isFill;
	Wing->SetMeshFill(isFill);
	Wing1->SetMeshFill(isFill);
	Wing2->SetMeshFill(isFill);
}

void TripleWindmillWing::SetCullTriangle(FInputValue Value)
{
	isCull = !isCull;
	Wing->SetMeshCull(isCull);
	Wing1->SetMeshCull(isCull);
	Wing2->SetMeshCull(isCull);
}