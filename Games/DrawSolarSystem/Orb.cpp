#include "Orb.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include "../../Engine/ForestPearlEngine/TransformComponent.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include <iostream>

#include <iostream>

void FPOrb::Initialize()
{
	Transform = new TransformCompoenent();
	Transform->transform.position.x = 0.0f;
	Transform->transform.position.y = 0.0f;
	Transform->transform.position.z = 0.0f;
					 
	Transform->transform.rotation.x = 0.0f;
	Transform->transform.rotation.y = 0.0f;
	Transform->transform.rotation.z = 0.0f;
					 
	Transform->transform.scale.x = 1.0f;
	Transform->transform.scale.y = 1.0f;
	Transform->transform.scale.z = 1.0f;

	Mesh = new MeshComponent("Triangle2", &(Transform->transform));

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	//Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Pressed, &Triangle::Move);

	//Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &Triangle::SetFillTriangel);
	//Controller->GetInputComponent().BindMethod("IA_SetCullTriangle", this, EKeyState::Down, &Triangle::SetCullTriangle);
}

void FPOrb::BeginPlay()
{
}

void FPOrb::Tick()
{
}

void FPOrb::SetOrbType(EOrbType Type)
{
	OrbType = Type;

	if (OrbType == EOrbType::Sun)
	{
		ObjectSize = 80.f;
		Speed = 20.f;
	}
	else if (OrbType == EOrbType::Planet)
	{
		ObjectSize = 60.f;
		Speed = 40.f;
	}
	else
	{
		ObjectSize = 30.f;
		Speed = 60.f;
	}
}
