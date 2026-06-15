#include "FPAController.h"
#include "Object/Components/InputComponent.h"

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
