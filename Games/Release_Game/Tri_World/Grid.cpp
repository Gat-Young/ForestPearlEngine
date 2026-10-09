#include "Grid.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/FPGizmoGridComponent.h"

void Grid::Initialize()
{
	GridComponets = new FPGizmoGridComponent(this, "Grid");
	SetRootComponent((FPSceneComponent*)GridComponets);
}

void Grid::BeginPlay()
{

}

void Grid::Tick()
{
	__super::Tick();
}

void Grid::SetActiveViewHelp(struct FInputValue Value)
{
	std::cout << "F2 : ";
	bShow = !(bShow);
	std::cout << bShow << "\n";
	GridComponets->SetActive(bShow);
}