#include "ACar.h"
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
	const char* CarPartMeshNames[] =
	{
		"Object_10_StaticMesh",
		"Object_13_StaticMesh",
		"Object_16_StaticMesh",
		"Object_19_StaticMesh",
		"Object_22_StaticMesh",
		"Object_25_StaticMesh",
		"Object_28_StaticMesh",
		"Object_31_StaticMesh",
		"Object_34_StaticMesh",
		"Object_37_StaticMesh",
		"Object_40_StaticMesh",
		"Object_43_StaticMesh",
		"Object_45_StaticMesh",
		"Object_48_StaticMesh",
		"Object_51_StaticMesh",
		"Object_54_StaticMesh",
		"Object_57_StaticMesh",
		"Object_60_StaticMesh",
		"Object_63_StaticMesh",
		"Object_66_StaticMesh",
		"Object_69_StaticMesh",
		"Object_72_StaticMesh",
		"Object_75_StaticMesh",
		"Object_78_StaticMesh",
		"Object_81_StaticMesh",
		"Object_84_StaticMesh",
		"Object_87_StaticMesh",
		"Object_90_StaticMesh",
		"Object_93_StaticMesh",
		"Object_96_StaticMesh",
		"Object_99_StaticMesh",
		"Object_102_StaticMesh",
		"Object_105_StaticMesh",
		"Object_108_StaticMesh",
		"Object_111_StaticMesh",
		"Object_113_StaticMesh",
		"Object_115_StaticMesh",
		"Object_118_StaticMesh",
		"Object_120_StaticMesh",
		"Object_123_StaticMesh",
		"Object_126_StaticMesh",
		"Object_129_StaticMesh",
		"Object_132_StaticMesh",
		"Object_135_StaticMesh",
		"Object_138_StaticMesh",
		"Object_141_StaticMesh",
		"Object_144_StaticMesh",
		"Object_147_StaticMesh",
		"Object_150_StaticMesh",
		"Object_153_StaticMesh",
		"Object_156_StaticMesh",
		"Object_159_StaticMesh",
		"Object_162_StaticMesh",
		"Object_165_StaticMesh",
		"Object_168_StaticMesh",
		"Object_171_StaticMesh",
		"Object_174_StaticMesh",
		"Object_177_StaticMesh",
		"Object_180_StaticMesh",
		"Object_183_StaticMesh",
		"Object_186_StaticMesh",
		"Object_189_StaticMesh",
		"Object_192_StaticMesh",
		"Object_195_StaticMesh",
		"Object_198_StaticMesh",
		"Object_201_StaticMesh",
		"Object_204_StaticMesh",
		"Object_207_StaticMesh",
		"Object_210_StaticMesh",
		"Object_213_StaticMesh",
		"Object_216_StaticMesh",
		"Object_219_StaticMesh",
		"Object_222_StaticMesh",
		"Object_225_StaticMesh",
		"Object_228_StaticMesh",
		"Object_231_StaticMesh",
		"Object_234_StaticMesh",
		"Object_237_StaticMesh",
		"Object_240_StaticMesh",
		"Object_243_StaticMesh",
		"Object_246_StaticMesh",
		"Object_249_StaticMesh",
		"Object_252_StaticMesh",
		"Object_255_StaticMesh",
		"Object_258_StaticMesh",
		"Object_261_StaticMesh",
		"Object_264_StaticMesh",
		"Object_267_StaticMesh",
		"Object_270_StaticMesh",
		"Object_273_StaticMesh",
		"Object_276_StaticMesh",
		"Object_279_StaticMesh",
		"Object_282_StaticMesh",
		"Object_285_StaticMesh",
		"Object_288_StaticMesh",
		"Object_291_StaticMesh",
		"Object_294_StaticMesh",
		"Object_297_StaticMesh",
		"Object_300_StaticMesh",
		"Object_303_StaticMesh",
		"Object_306_StaticMesh",
		"Object_309_StaticMesh",
		"Object_312_StaticMesh",
		"Object_315_StaticMesh",
		"Object_318_StaticMesh",
		"Object_321_StaticMesh",
		"Object_324_StaticMesh",
		"Object_327_StaticMesh",
		"Object_330_StaticMesh",
		"Object_333_StaticMesh",
		"Object_336_StaticMesh",
		"Object_339_StaticMesh",
		"Object_342_StaticMesh",
		"Object_345_StaticMesh",
		"Object_348_StaticMesh",
		"Object_351_StaticMesh",
		"Object_354_StaticMesh",
		"Object_357_StaticMesh",
	};
}

void ACar::Initialize()
{
	for (const char* MeshName : CarPartMeshNames)
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

		CarParts.push_back(Part);
	}

	SpringArm = new FPSpringArmComponent(this);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 15.0f;
	SpringArm->SetRelativeRotation({ 20.0f, 0.0f, 0.0f });
	SpringArm->bUsePawnControlRotation = true;

	PlayerCamera = new FPCameraComponent(this);
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->Zf = 1000.0f;

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &ACar::CameraMove);
	Controller->GetInputComponent().BindMethod("IA_ZoomCamera", this, EKeyState::Pressed, &ACar::CameraZoom);

	Controller->Possess(this);
}

void ACar::BeginPlay()
{
}

void ACar::Tick()
{
	__super::Tick();
}

void ACar::CameraMove(FInputValue Value)
{
	float mov = 60.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	AddControllerYawInput(-move_x);
	AddControllerPitchInput(move_y);
}

void ACar::CameraZoom(FInputValue Value)
{
	float mov = 10.0f;
	float mov_size = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	SpringArm->TargetArmLength = SpringArm->TargetArmLength + mov_size;

	if (SpringArm->TargetArmLength < 2.0f)
		SpringArm->TargetArmLength = 2.0f;
}
