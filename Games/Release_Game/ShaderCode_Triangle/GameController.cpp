#include "GameController.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"

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
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 100 , ModifyInfoD);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 101, ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetScaleWing", 106, ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetScaleWing", 107, ModifyInfoD);

	//Action Button : Game Pad
	GetInputComponent().AddMappingKey("IA_AttachHead", 102, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_AttachShield", 103, ModifyInfoTriger);

	//Action Button : KeyBoard
	GetInputComponent().AddMappingKey("IA_SetFillTriangel", VK_SPACE, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetCullTriangel", VK_F4, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetUITriangel", VK_F1, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_SetGrid", VK_F2, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetAxis", VK_F3, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_SetDepthStencilBuffer", VK_F5, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_AttachHead", 'Z', ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_AttachShield", 'X', ModifyInfoTriger);



	__super::Initialize();
}

void GameController::BeginPlay()
{
	__super::BeginPlay();
}

void GameController::Tick()
{
	__super::Tick();
}
