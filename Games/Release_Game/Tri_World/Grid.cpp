#include "Grid.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/GameTimer.h"
#include "ForestPearlEngine/FPWorld.h"

void Grid::Initialize()
{
	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetGrid", this, EKeyState::Down, &Grid::SetActiveViewHelp);

	GridComponets = new GizmoComponent(this);

	//Grid ¸¸µé±â
	GRIDINFO grid;
	grid.width = 100;
	grid.height = 100;

	GridComponets->MakeGrid(&grid);

	SetRootComponent((FPSceneComponent*)GridComponets);
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