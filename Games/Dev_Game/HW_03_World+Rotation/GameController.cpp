#include "GameController.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"

void GameController::Initialize()
{
	FModifyInfo ModifyInfoA = { ESwizzle::XYZ , ENegative::Negative };
	FModifyInfo ModifyInfoTriger = { ESwizzle::XYZ , ENegative::Positive };

	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'A', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetFillTriangel", VK_SPACE, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetCullTriangel", VK_F5, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetUITriangel", VK_F1, ModifyInfoTriger);

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
