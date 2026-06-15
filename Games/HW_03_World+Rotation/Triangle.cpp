#include "Triangle.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputMappingContext.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputAction.h"
#include "../../Engine/ForestPearlEngine/Systems/InputSystem.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include <iostream>

void Triangle::Initialize()
{
	Transform = new TransformCompoenent();
	Transform->transfrom.position.x = 30.0f;
	Transform->transfrom.position.y = 0.0f;
	Transform->transfrom.position.z = 0.0f;

	Transform->transfrom.rotation.x = 0.0f;
	Transform->transfrom.rotation.y = 0.0f;
	Transform->transfrom.rotation.z = 0.0f;

	Transform->transfrom.scale.x = 1.0f;
	Transform->transfrom.scale.y = 1.0f;
	Transform->transfrom.scale.z = 1.0f;

	Mesh = new MeshComponent("Triangle2", &(Transform->transfrom));
	IMC = new FPInputMappingContext();
	FPInputSystem::GetInputSystem().AddActivatedIMC(IMC);

	IA = new FPInputAction();
	IA->BindMethod(this, EKeyState::Down, &Triangle::SetFillTriangel);
	FMappingInfo MappingInfoSpace = { IA,0b00000100 };
	IMC->AddMappingKey(VK_SPACE, MappingInfoSpace);

	IMC2 = new FPInputMappingContext();
	FPInputSystem::GetInputSystem().AddActivatedIMC(IMC2);
	IA2 = new FPInputAction();

	IA2->BindMethod(this, EKeyState::Down, &Triangle::SetCullTriangle);
	FMappingInfo MappingInfoSpace2 = { IA2,0b00000100 };
	IMC2->AddMappingKey(VK_F5, MappingInfoSpace2);
}

void Triangle::BeginPlay()
{
}

void Triangle::Tick()
{
	angle += 3.141592f * 0.5f * 1/GetWorld()->GetGameTimer()->DeltaTimeMS();
	Transform->transfrom.rotation.z = angle;
}

void Triangle::Move(FPVector2 Value)
{
	std::cout << "Move Begin!! [ " << Value.x << " : " << Value.y << " ]\n";
}

void Triangle::SetFillTriangel(FPVector2 Value)
{
	std::cout << "FillMode : " << isFill << "\n";
	isFill = !isFill;
	Mesh->SetMeshFill(isFill);
}

void Triangle::SetCullTriangle(FPVector2 Value)
{
	std::cout << "CullMode : " << isCull << "\n";
	isCull = !isCull;
	Mesh->SetMeshCull(isCull);
}

