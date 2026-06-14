#include "Triangle.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputMappingContext.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputAction.h"
#include "../../Engine/ForestPearlEngine/Systems/InputSystem.h"
#include <iostream>

void Triangle::Initialize()
{
	Mesh = new MeshComponent("Triangle");
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

	//FMappingInfo MappingInfoW = { IA , 0b00000100 };
	//FMappingInfo MappingInfoA = { IA , 0b00001001 };
	//FMappingInfo MappingInfoS = { IA , 0b00000101 };
	//FMappingInfo MappingInfoD = { IA , 0b00001000 };

	//IMC->AddMappingKey('W', MappingInfoW);
	//IMC->AddMappingKey('A', MappingInfoA);
	//IMC->AddMappingKey('S', MappingInfoS);
	//IMC->AddMappingKey('D', MappingInfoD);
}

void Triangle::BeginPlay()
{
}

void Triangle::Tick()
{
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

