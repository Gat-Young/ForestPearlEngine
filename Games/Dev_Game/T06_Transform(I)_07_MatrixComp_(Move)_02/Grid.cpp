#include "Grid.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"

void Grid::Initialize()
{
	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetGrid", this, EKeyState::Pressed, &Grid::SetActiveViewHelp);

	GridComponets = new GizmoComponent();

	GRIDINFO grid;
	grid.width = 10;
	grid.height = 10;

	GridComponets->MakeGrid(&grid);

	GridComponets->RegistGizmoRenderList(&Position, &Rotation, &Scale);
}

void Grid::BeginPlay()
{

}

void Grid::Tick()
{

}

void Grid::SetActiveViewHelp(struct FInputValue Value)
{
	std::cout << "F1 : ";
	bShow = !(bShow);
	std::cout << bShow << "\n";
	GridComponets->SetActive(bShow);
}