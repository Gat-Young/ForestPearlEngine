#include "GameController.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include <iostream>

void GameController::Initialize()
{
	FModifyInfo ModifyInfoA = { ESwizzle::XYZ , ENegative::Negative };
	FModifyInfo ModifyInfoD = { ESwizzle::XYZ , ENegative::Positive };
	FModifyInfo ModifyInfoW = { ESwizzle::YZX , ENegative::Positive };
	FModifyInfo ModifyInfoS = { ESwizzle::YZX , ENegative::Negative };
	FModifyInfo ModifyInfoTriger = { ESwizzle::XYZ , ENegative::Positive };

	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'W', ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'A', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'S', ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'D', ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'I', ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'J', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'K', ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'L', ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetMoveWindmill", VK_UP, ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetMoveWindmill", VK_LEFT, ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetMoveWindmill", VK_DOWN, ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetMoveWindmill", VK_RIGHT, ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetRotateWindmill", 'E', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetRotateWindmill", 'Q', ModifyInfoD);
	GetInputComponent().AddMappingKey("IA_SetScaleWindmill", 'F', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetScaleWindmill", 'R', ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetScaleWing", VK_OEM_COMMA, ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetScaleWing", VK_OEM_PERIOD, ModifyInfoD);

	// Axis : Game Pad 
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 0x100 , ModifyInfoD);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 0x101, ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetScaleWing", 0x106, ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetScaleWing", 0x107, ModifyInfoD);

	//Action Button : Game Pad
	GetInputComponent().AddMappingKey("IA_AttachHead", 0x102, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_AttachShield", 0x103, ModifyInfoTriger);

	//Action Button : KeyBoard
	GetInputComponent().AddMappingKey("IA_SetFillTriangel", VK_SPACE, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetCullTriangel", VK_F4, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetUITriangel", VK_F1, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_SetGrid", VK_F2, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetAxis", VK_F3, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_SetDepthStencilBuffer", VK_F5, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_AttachHead", 'Z', ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_AttachShield", 'X', ModifyInfoTriger);

	//D-PAD LEFT/RIGHT Posses 전환
	GetInputComponent().AddMappingKey("IA_NextActor", 0x10D, ModifyInfoTriger); //RIGHT
	GetInputComponent().AddMappingKey("IA_PrevActor", 0x10C, ModifyInfoTriger); //LEFT


	//Posses 바인딩
	GetInputComponent().BindMethod("IA_NextActor", this, EKeyState::Down, &GameController::NextPawn);
	GetInputComponent().BindMethod("IA_PrevActor", this, EKeyState::Down, &GameController::PrevPawn);

	__super::Initialize();
}

void GameController::BeginPlay()
{
	__super::BeginPlay();
	ControllPawn.push_back(static_cast<FPPawn*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "Player")));
	ControllPawn.push_back(static_cast<FPPawn*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "Windmill")));
	ControllPawn.push_back(static_cast<FPPawn*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "TripleWindmillWing")));
	ControllPawnSize = ControllPawn.size();
}

void GameController::Tick()
{
	__super::Tick();
}

void GameController::NextPawn(FInputValue value)
{
	std::cout << "[GameController] : NextPawn" <<  "\n";
	ControllPawnIndex++;
	ControllPawnIndex %= ControllPawnSize;
	std::cout << "[GameController] : " << ControllPawnIndex << " : " << ControllPawnSize << "\n";
	Possess(ControllPawn[ControllPawnIndex]);
}

void GameController::PrevPawn(FInputValue value)
{
	std::cout << "[GameController] : PrevPawn" <<  "\n";
	ControllPawnIndex--;
	if (ControllPawnIndex < 0)
	{
		ControllPawnIndex = ControllPawnSize - 1;
	}
	std::cout << "[GameController] : " << ControllPawnIndex << " : " << ControllPawnSize << "\n";
	Possess(ControllPawn[ControllPawnIndex]);
}
