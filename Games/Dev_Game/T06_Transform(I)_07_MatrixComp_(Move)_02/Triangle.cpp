#include "Triangle.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include <iostream>

void Triangle::Initialize()
{

	Mesh = new MeshComponent(this ,"Triangle3");
	SetRootComponent((FPSceneComponent*)Mesh);

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &Triangle::Move);

	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &Triangle::SetFillTriangel);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &Triangle::SetCullTriangle);




	//FMappingInfo MappingInfoSpace = { IA,0b00000100 };
	//IMC->AddMappingKey(VK_SPACE, MappingInfoSpace);

	//IMC2 = new FPInputMappingContext();
	//FPInputSystem::GetInputSystem().AddActivatedIMC(IMC2);

	//FMappingInfo MappingInfoSpace2 = { IA2,0b00000100 };
	//IMC2->AddMappingKey(VK_F5, MappingInfoSpace2);
}

void Triangle::BeginPlay()
{
}

void Triangle::Tick()
{
	angle += 1/(3.141592f * GetWorld()->GetGameTimer()->DeltaTimeMS());
	RootComponent->SetRelativeRotation(FPVector3{ 0.0f, angle, 0.0f });

	__super::Tick();
}

void Triangle::Move(FInputValue Value)
{
	std::cout << "Move Begin!! [ " << Value.X << " : " << Value.Y << " ]\n";
	float mov = 0.5f;
	float move_x = Value.X * mov * 1 / (GetWorld()->GetGameTimer()->DeltaTimeMS());
	float move_y = Value.Y * mov * 1 / (GetWorld()->GetGameTimer()->DeltaTimeMS());
	std::cout << "Move Begin!! [ " << move_x << " : " << move_y << " ]\n";
	RootComponent->SetRelativeLocation(RootComponent->GetRelativeLocation() + FPVector3{move_x, 0.0f, move_y});
}

void Triangle::SetFillTriangel(FInputValue Value)
{
	std::cout << "FillMode : " << isFill << "\n";
	isFill = !isFill;
	Mesh->SetMeshFill(isFill);
}

void Triangle::SetCullTriangle(FInputValue Value)
{
	std::cout << "CullMode : " << isCull << "\n";
	isCull = !isCull;
	Mesh->SetMeshCull(isCull);
}

