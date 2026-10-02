#include "FPAController.h"
#include "Object/Components/InputComponent.h"
#include "FPSceneComponent.h"
#include "../ForestPearlEngine/Object/FPPawn.h"
#include <iostream>

FPAController::FPAController()
{
	InputComponent = new FPInputComponent(this);
	ControlRotation = FPVector3{ 0.0f ,0.0f, 0.0f };
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
	RootComponent->SetWorldRotation(ControlRotation);
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

FPPawn* FPAController::GetPawn()
{
	return this->PossessedPawn;
}

void FPAController::AddYawInput(float Value)
{
	ControlRotation.y += Value;
	ControlRotation.y = NormalizeAxis(ControlRotation.y);
}

void FPAController::AddPitchInput(float Value)
{
	ControlRotation.x += Value;
	ControlRotation.x = NormalizeAxis(ControlRotation.x);
}

void FPAController::AddRollInput(float Value)
{
	ControlRotation.z += Value;
	ControlRotation.z = NormalizeAxis(ControlRotation.z);
}
