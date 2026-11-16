#include "GameController.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"

void GameController::Initialize()
{
	FModifyInfo ModifyInfoJ = { ESwizzle::XYZ , ENegative::Negative };
	FModifyInfo ModifyInfoL = { ESwizzle::XYZ , ENegative::Positive };
	FModifyInfo ModifyInfoI = { ESwizzle::YZX , ENegative::Positive };
	FModifyInfo ModifyInfoK = { ESwizzle::YZX , ENegative::Negative };

	// Orbit camera around the car (IJKL)
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'I', ModifyInfoI);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'J', ModifyInfoJ);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'K', ModifyInfoK);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", 'L', ModifyInfoL);

	// Zoom camera with the mouse wheel
	GetInputComponent().AddMappingKey("IA_ZoomCamera", static_cast<USHORT>(CUSTOM_MOUSE_INPUT::MOUSE_WHEEL), ModifyInfoJ);

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
