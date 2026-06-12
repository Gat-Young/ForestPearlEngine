#include "GameController.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputAction.h"

void GameController::Initialize()
{
	FMappingInfo MappingInfoW = { ESwizzle::YZX , ENegative::Positive };
	FMappingInfo MappingInfoA = { ESwizzle::XYZ , ENegative::Negative };
	FMappingInfo MappingInfoS = { ESwizzle::YZX , ENegative::Negative };
	FMappingInfo MappingInfoD = { ESwizzle::XYZ , ENegative::Positive };
	GetInputComponent().AddMappingKey('W', MappingInfoW);
	GetInputComponent().AddMappingKey('A', MappingInfoA);
	GetInputComponent().AddMappingKey('S', MappingInfoS);
	GetInputComponent().AddMappingKey('D', MappingInfoD);

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
