#include "Terrain.h"
#include "ForestPearlEngine/FPMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"

void Terrain::Initialize()
{
	Mesh = new FPMeshComponent(this, "Terrain/Terrain.fbx");
	Mesh->SetTopology(TRIANGLELIST); // <- Topology를 변경할 수 있음

	SetRootComponent((FPSceneComponent*)Mesh);

	Mesh->SetMeshCull(false);
	FPAController* Controller = GetWorld()->GetController(0);

	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &Terrain::SetFillTriangel);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &Terrain::SetCullTriangle);

}

void Terrain::BeginPlay()
{

}

void Terrain::Tick()
{
	__super::Tick();
}

void Terrain::SetFillTriangel(FInputValue Value)
{
	isFill = !isFill;
	Mesh->SetMeshFill(isFill);
}

void Terrain::SetCullTriangle(FInputValue Value)
{
	isCull = !isCull;
	Mesh->SetMeshCull(isCull);
}