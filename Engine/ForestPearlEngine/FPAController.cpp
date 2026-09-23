#include "FPAController.h"
#include "Object/Components/InputComponent.h"
#include <iostream>

FPAController::FPAController()
{
	InputComponent = new FPInputComponent();
}

FPAController::~FPAController()
{
}

void FPAController::Initialize()
{
}

void FPAController::BeginPlay()
{
}

void FPAController::Tick()
{
	InputComponent->ProcessInputTick();
}

void FPAController::Possess(FPPawn* PossessedPawn)
{
	if (this->PossessedPawn) { this->PossessedPawn->UnPossesed(); }

	this->PossessedPawn = PossessedPawn;
	this->PossessedPawn->PossessedBy(this);
	InputComponent->Possess(PossessedPawn);
}

void FPAController::UnPossess()
{
	if (this->PossessedPawn) { this->PossessedPawn->UnPossesed(); }
	this->PossessedPawn = nullptr;
	InputComponent->UnPossess();
}
