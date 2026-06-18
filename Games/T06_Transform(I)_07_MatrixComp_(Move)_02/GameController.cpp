#include "GameController.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"

void GameController::Initialize()
{
	FModifyInfo ModifyInfoW = { ESwizzle::YZX , ENegative::Positive };
	FModifyInfo ModifyInfoA = { ESwizzle::XYZ , ENegative::Negative };
	FModifyInfo ModifyInfoS = { ESwizzle::YZX , ENegative::Negative };
	FModifyInfo ModifyInfoD = { ESwizzle::XYZ , ENegative::Positive };

	FModifyInfo ModifyInfoTriger = { ESwizzle::XYZ , ENegative::Positive };

	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'W', ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'A', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'S', ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'D', ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetFillTriangel", VK_SPACE, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetCullTriangel", VK_F5, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetUITriangel", VK_F1, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_SetGrid", VK_F2, ModifyInfoTriger);

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
