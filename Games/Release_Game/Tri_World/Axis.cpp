#include "Axis.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/GizmoAxisComponent.h"

void Axis::Initialize()
{


	AxisComponets = new GizmoAxisComponent(this, "Axis");


	SetRootComponent((FPSceneComponent*)AxisComponets);
}

void Axis::BeginPlay()
{

}

void Axis::Tick()
{
	__super::Tick();
}

void Axis::SetActiveViewHelp(struct FInputValue Value)
{
	std::cout << "F3 : ";
	bShow = !(bShow);
	std::cout << bShow << "\n";
	AxisComponets->SetActive(bShow);
}