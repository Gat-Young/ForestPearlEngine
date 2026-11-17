#include "GameController.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ASkyLight.h"
#include "ASkyBox.h"
#include <vector>

void GameController::Initialize()
{
	FModifyInfo ModifyInfoA = { ESwizzle::XYZ , ENegative::Negative };
	FModifyInfo ModifyInfoD = { ESwizzle::XYZ , ENegative::Positive };
	FModifyInfo ModifyInfoW = { ESwizzle::YZX , ENegative::Positive };
	FModifyInfo ModifyInfoS = { ESwizzle::YZX , ENegative::Negative };
	FModifyInfo ModifyInfoTriger = { ESwizzle::XYZ , ENegative::Positive };

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

	// Toggle SkyLight/SkyBox environment map (F9)
	GetInputComponent().AddMappingKey("IA_SetEnvironmentMap", VK_F9, ModifyInfoTriger);
	GetInputComponent().BindMethod("IA_SetEnvironmentMap", this, EKeyState::Down, &GameController::SetEnvironmentMap);

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

void GameController::SetEnvironmentMap(FInputValue Value)
{
	std::string NewEnvironmentMap;

	std::vector<FPActor*> ActorList;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "ASkyLight", ActorList);
	for (FPActor* Actor : ActorList)
	{
		ASkyLight* SkyLight = static_cast<ASkyLight*>(Actor);
		NewEnvironmentMap = SkyLight->ToggleEnvironmentMap();
	}
	ActorList.clear();

	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "ASkyBox", ActorList);
	for (FPActor* Actor : ActorList)
	{
		ASkyBox* SkyBox = static_cast<ASkyBox*>(Actor);
		SkyBox->SetEnvironmentMap(NewEnvironmentMap);
	}
	ActorList.clear();
}
