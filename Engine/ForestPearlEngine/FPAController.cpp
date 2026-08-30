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

void FPAController::Possess(FPActor* PossessActor)
{
	this->PossesedActor = PossessActor;
	InputComponent->Possess(PossessActor);
}

void FPAController::UnPossess()
{
	this->PossesedActor = nullptr;
	InputComponent->UnPossess();
}
