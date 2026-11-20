#include "GameController.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/Systems/InputSystem.h"

void GameController::Initialize()
{
	FModifyInfo ModifyInfoA = { ESwizzle::XYZ , ENegative::Negative };
	FModifyInfo ModifyInfoD = { ESwizzle::XYZ , ENegative::Positive };
	FModifyInfo ModifyInfoW = { ESwizzle::YZX , ENegative::Positive };
	FModifyInfo ModifyInfoS = { ESwizzle::YZX , ENegative::Negative };

	// Move (WASD)
	GetInputComponent().AddMappingKey("IA_SetMovePlayer", 'W', ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetMovePlayer", 'A', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetMovePlayer", 'S', ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetMovePlayer", 'D', ModifyInfoD);

	// Rotate Camera (IJKL)
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'I', ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'J', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'K', ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'L', ModifyInfoD);

	// Zoom Camera (Mouse Wheel)
	GetInputComponent().AddMappingKey("IA_ZoomCamera", static_cast<USHORT>(CUSTOM_MOUSE_INPUT::MOUSE_WHEEL), ModifyInfoA);

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
