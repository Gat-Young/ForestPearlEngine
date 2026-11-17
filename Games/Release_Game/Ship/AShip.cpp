#include "AShip.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPSpringArmComponent.h"

namespace
{
	const char* ShipPartMeshNames[] =
	{
		"Object_2_StaticMesh",
		"Object_3_StaticMesh",
		"Object_4_StaticMesh",
		"Object_5_StaticMesh",
		"Object_6_StaticMesh",
		"Object_7_StaticMesh",
		"Object_8_StaticMesh",
		"Object_9_StaticMesh",
		"Object_10_StaticMesh",
		"Object_11_StaticMesh",
		"Object_12_StaticMesh",
		"Object_13_StaticMesh",
		"Object_14_StaticMesh",
		"Object_15_StaticMesh",
		"Object_16_StaticMesh",
		"Object_17_StaticMesh",
		"Object_18_StaticMesh",
		"Object_19_StaticMesh",
		"Object_20_StaticMesh",
		"Object_21_StaticMesh",
		"Object_22_StaticMesh",
		"Object_23_StaticMesh",
		"Object_24_StaticMesh",
		"Object_25_StaticMesh",
		"Object_26_StaticMesh",
		"Object_27_StaticMesh",
		"Object_28_StaticMesh",
		"Object_29_StaticMesh",
		"Object_30_StaticMesh",
		"Object_31_StaticMesh",
		"Object_32_StaticMesh",
		"Object_33_StaticMesh",
		"Object_34_StaticMesh",
		"Object_35_StaticMesh",
		"Object_36_StaticMesh",
		"Object_37_StaticMesh",
		"Object_38_StaticMesh",
		"Object_39_StaticMesh",
		"Object_40_StaticMesh",
		"Object_41_StaticMesh",
		"Object_42_StaticMesh",
	};
}

void AShip::Initialize()
{
	for (const char* MeshName : ShipPartMeshNames)
	{
		FPStaticMeshComponent* Part = new FPStaticMeshComponent(this, MeshName);

		if (RootComponent == nullptr)
		{
			SetRootComponent((FPSceneComponent*)Part);
		}
		else
		{
			Part->SetupAttachment(RootComponent);
		}

		ShipParts.push_back(Part);
	}

	SpringArm = new FPSpringArmComponent(this);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 40.0f;
	SpringArm->SetRelativeRotation({ 20.0f, 0.0f, 0.0f });
	SpringArm->bUsePawnControlRotation = true;

	PlayerCamera = new FPCameraComponent(this);
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->Zf = 1000.0f;

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &AShip::CameraMove);

	Controller->Possess(this);
}

void AShip::BeginPlay()
{
}

void AShip::Tick()
{
	__super::Tick();
}

void AShip::CameraMove(FInputValue Value)
{
	float mov = 60.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	AddControllerYawInput(-move_x);
	AddControllerPitchInput(move_y);
}
