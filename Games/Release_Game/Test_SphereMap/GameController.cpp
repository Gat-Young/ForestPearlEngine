#include "GameController.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"

void GameController::Initialize()
{
	FModifyInfo ModifyInfoA = { ESwizzle::XYZ , ENegative::Negative };
	FModifyInfo ModifyInfoD = { ESwizzle::XYZ , ENegative::Positive };
	FModifyInfo ModifyInfoW = { ESwizzle::YZX , ENegative::Positive };
	FModifyInfo ModifyInfoS = { ESwizzle::YZX , ENegative::Negative };

	// Move UtaTeapot (WASD)
	GetInputComponent().AddMappingKey("IA_SetMoveTeapot", 'W', ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetMoveTeapot", 'A', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetMoveTeapot", 'S', ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetMoveTeapot", 'D', ModifyInfoD);

	// Rotate UtaTeapot (IJKL)
	GetInputComponent().AddMappingKey("IA_SetRotateTeapot", 'I', ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetRotateTeapot", 'J', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetRotateTeapot", 'K', ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetRotateTeapot", 'L', ModifyInfoD);

	// Orbit camera around UtaTeapot (Arrow keys)
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", VK_UP, ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", VK_LEFT, ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", VK_DOWN, ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", VK_RIGHT, ModifyInfoD);

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
