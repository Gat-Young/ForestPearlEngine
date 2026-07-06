#include "Triangle.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include "../../Engine/ForestPearlEngine/TransformComponent.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include <iostream>

void Triangle::Initialize()
{
	Transform = new TransformCompoenent();
	Transform->transfrom.position.x = 0.0f;
	Transform->transfrom.position.y = 0.0f;
	Transform->transfrom.position.z = 0.0f;

	Transform->transfrom.rotation.x = 0.0f;
	Transform->transfrom.rotation.y = 0.0f;
	Transform->transfrom.rotation.z = 0.0f;

	Transform->transfrom.scale.x = 1.0f;
	Transform->transfrom.scale.y = 1.0f;
	Transform->transfrom.scale.z = 1.0f;

	Mesh = new MeshComponent("Triangle2", &(Transform->transfrom));

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
	angle += 10.0f;//3.141592f;//  *1 / GetWorld()->GetGameTimer()->DeltaTimeMS();
	Transform->transfrom.rotation.z = angle;
	//std::cout << "angle : " << angle << "\n";
}

void Triangle::Move(FInputValue Value)
{
	std::cout << "Move Begin!! [ " << Value.X << " : " << Value.Y << " ]\n";
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

