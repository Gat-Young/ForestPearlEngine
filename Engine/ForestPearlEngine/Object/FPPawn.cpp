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
