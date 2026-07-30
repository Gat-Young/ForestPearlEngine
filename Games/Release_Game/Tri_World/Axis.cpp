#include "Axis.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPWorld.h"

void Axis::Initialize()
{
	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetAxis", this, EKeyState::Down, &Axis::SetActiveViewHelp);

	AxisComponets = new GizmoComponent(this);

	//Axis ¸¸µé±â
	GIZMO_AXISINFO axis;

	AxisComponets->MakeAxis(&axis);

	AxisComponets->SetPriority(0);
	SetRootComponent((FPSceneComponent*)AxisComponets);
}

void Axis::BeginPlay()
{

}

void Axis::Tick()
{

}

void Axis::SetActiveViewHelp(struct FInputValue Value)
{
	std::cout << "F1 : ";
	bShow = !(bShow);
	std::cout << bShow << "\n";
	AxisComponets->SetActive(bShow);
}