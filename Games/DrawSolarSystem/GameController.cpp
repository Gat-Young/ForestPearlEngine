#include "GameController.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputAction.h"

void GameController::Initialize()
{
	FModifyInfo ModifyInfoW = { ESwizzle::YZX , ENegative::Positive };
	FModifyInfo ModifyInfoA = { ESwizzle::XYZ , ENegative::Negative };
	FModifyInfo ModifyInfoS = { ESwizzle::YZX , ENegative::Negative };
	FModifyInfo ModifyInfoD = { ESwizzle::XYZ , ENegative::Positive };
	//GetInputComponent().AddMappingKey("IA_Move", VK_LBUTTON, ModifyInfoD);
	GetInputComponent().AddMappingKey("IA_Move", 'W', ModifyInfoW);
	GetInputComponent().AddMappingKey("IA_Move", 'A', ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_Move", 'S', ModifyInfoS);
	GetInputComponent().AddMappingKey("IA_Move", 'D', ModifyInfoD);

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
