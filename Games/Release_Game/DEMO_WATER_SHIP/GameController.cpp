#include "GameController.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/Systems/InputSystem.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPWorld.h"
#include "Cannon.h"
#include "Frigate.h"
#include <vector>

namespace
{
	bool IsDown(USHORT Key)
	{
		FPInputSystem* Input = static_cast<FPInputSystem*>(FPGameInstance::Get().GetInputSystem());
		return Input != nullptr && Input->bIsKeyDown(Key);
	}
}

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

	// Fire Cannon: Enter alone fires every Cannon; Enter+1/2/3 fires a Frigate broadside (All/Left/Right)
	FModifyInfo ModifyInfoTrigger = { ESwizzle::XYZ , ENegative::Positive };
	GetInputComponent().AddMappingKey("IA_FireCannon", VK_RETURN, ModifyInfoTrigger);
	GetInputComponent().AddMappingKey("IA_FireCannon", '1', ModifyInfoTrigger);
	GetInputComponent().AddMappingKey("IA_FireCannon", '2', ModifyInfoTrigger);
	GetInputComponent().AddMappingKey("IA_FireCannon", '3', ModifyInfoTrigger);
	GetInputComponent().BindMethod("IA_FireCannon", this, EKeyState::Down, &GameController::FireCannon);

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

void GameController::FireCannon(FInputValue Value)
{
	if (!IsDown(VK_RETURN)) return;	//number key alone : do nothing

	Frigate::ESide Side = Frigate::ESide::All;
	if (IsDown('2')) Side = Frigate::ESide::Left;
	else if (IsDown('3')) Side = Frigate::ESide::Right;

	std::vector<FPActor*> Frigates;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "Frigate", Frigates);
	for (FPActor* Actor : Frigates) static_cast<Frigate*>(Actor)->Fire(Side);

	std::vector<FPActor*> Cannons;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "Cannon", Cannons);
	for (FPActor* Actor : Cannons) static_cast<Cannon*>(Actor)->Fire();
}
