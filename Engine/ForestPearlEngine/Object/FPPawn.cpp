#include "FPPawn.h"
#include "../FPAController.h"

FPAController* FPPawn::GetController() const
{
	if (Controller == nullptr) return nullptr;
	return Controller;
}

void FPPawn::PossessedBy(FPAController* NewController)
{
	Controller = NewController;
}

void FPPawn::UnPossesed()
{
	Controller = nullptr;
}

void FPPawn::AddControllerYawInput(float Value)
{
	FPAController* Controller = GetController();
	if (Controller)
	{
		Controller->AddYawInput(Value);
	}
}

void FPPawn::AddControllerPitchInput(float Value)
{
	FPAController* Controller = GetController();
	if (Controller)
	{
		Controller->AddPitchInput(Value);
	}
}

void FPPawn::AddControllerRollInput(float Value)
{
	FPAController* Controller = GetController();
	if (Controller)
	{
		Controller->AddRollInput(Value);
	}
}
