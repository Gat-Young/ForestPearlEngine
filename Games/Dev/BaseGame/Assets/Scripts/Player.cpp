#include "Player.h"
#include "../../../../Engine/ForestPearlEngine/Object/Components/InputMappingContext.h"
#include "../../../../Engine/ForestPearlEngine/Object/Components/InputAction.h"
#include "../../../../Engine/ForestPearlEngine/Systems/InputSystem.h"
#include <iostream>

Player::Player()
{
	IMC = new FPInputMappingContext();
	FPInputSystem::GetInputSystem().AddActivatedIMC(IMC);

	IA = new FPInputAction();
	IA->BindMethod(this, EKeyState::Down, &Player::Move);

	FMappingInfo MappingInfoW = { IA , 0b00000100 };
	FMappingInfo MappingInfoA = { IA , 0b00001001 };
	FMappingInfo MappingInfoS = { IA , 0b00000101 };
	FMappingInfo MappingInfoD = { IA , 0b00001000 };
	IMC->AddMappingKey('W', MappingInfoW);
	IMC->AddMappingKey('A', MappingInfoA);
	IMC->AddMappingKey('S', MappingInfoS);
	IMC->AddMappingKey('D', MappingInfoD);
}

Player::~Player()
{
	FPInputSystem::GetInputSystem().RemoveActivatedIMC(IMC);
	delete IMC;
	delete IA;
}

void Player::BeginPlay()
{

}

void Player::Tick()
{
	//std::cout << "³­ Player ¾ß!" << "\n";
}

void Player::Move(FPVector2 Value)
{
	std::cout << "Move Begin!! [ " << Value.x << " : " << Value.y << " ]\n";
}
