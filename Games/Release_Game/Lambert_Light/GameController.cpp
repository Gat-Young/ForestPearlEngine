#include "GameController.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"

//Fill/ Cull을 위해 엔진을 가져온
#include "ForestPearlEngine/ForestPearlEngine.h"

//UI 처리
#include "UI.h"
#include "Axis.h"
#include "Grid.h"

#include <iostream>

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
	GetInputComponent().AddMappingKey("IA_SetMoveTriangel", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_LSTICK), ModifyInfoD);
	GetInputComponent().AddMappingKey("IA_SetMoveCamera", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_RSTICK), ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetMoveWindmill", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_LSTICK), ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetScaleWindmill", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_LEFT_TRIGER), ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetScaleWindmill", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_RIGHT_TRIGER), ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetScaleWing", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_LEFT_SHOULDER), ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetScaleWing", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_RIGHT_SHOULDER), ModifyInfoD);

	GetInputComponent().AddMappingKey("IA_SetRotateWindmill", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_LEFT_THUMB), ModifyInfoA);
	GetInputComponent().AddMappingKey("IA_SetRotateWindmill", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_RIGHT_THUMB), ModifyInfoD);

	//Action Button : Game Pad
	GetInputComponent().AddMappingKey("IA_AttachHead", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_A), ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_AttachShield", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_B), ModifyInfoTriger);

	//Action Button : KeyBoard
	GetInputComponent().AddMappingKey("IA_SetFillTriangel", VK_SPACE, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetCullTriangel", VK_F4, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetUITriangel", VK_F1, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_SetGrid", VK_F2, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetAxis", VK_F3, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_SetDepthStencilBuffer", VK_F5, ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_SetNormalLine", VK_F6, ModifyInfoTriger);

	GetInputComponent().AddMappingKey("IA_AttachHead", 'Z', ModifyInfoTriger);
	GetInputComponent().AddMappingKey("IA_AttachShield", 'X', ModifyInfoTriger);

	//D-PAD LEFT/RIGHT Posses 전환
	GetInputComponent().AddMappingKey("IA_NextActor", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_DPAD_RIGHT), ModifyInfoTriger); //RIGHT
	GetInputComponent().AddMappingKey("IA_PrevActor", static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_DPAD_LEFT), ModifyInfoTriger); //LEFT


	//Posses 바인딩
	GetInputComponent().BindMethod("IA_NextActor", this, EKeyState::Down, &GameController::NextPawn);
	GetInputComponent().BindMethod("IA_PrevActor", this, EKeyState::Down, &GameController::PrevPawn);

	//Fill / Cull 바인딩
	GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &GameController::SetFillTriangel);
	GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &GameController::SetCullTriangle);

	//UI
	GetInputComponent().BindMethod("IA_SetUITriangel", this, EKeyState::Down, &GameController::SetActiveViewHelp);
	GetInputComponent().BindMethod("IA_SetDepthStencilBuffer", this, EKeyState::Down, &GameController::SetActiveDepthStencilBuffer);
	GetInputComponent().BindMethod("IA_SetGrid", this, EKeyState::Down, &GameController::SetGridOn);
	GetInputComponent().BindMethod("IA_SetAxis", this, EKeyState::Down, &GameController::SetAxisOn);
	GetInputComponent().BindMethod("IA_SetNormalLine", this, EKeyState::Down, &GameController::SetNormalLine);


	__super::Initialize();
}

void GameController::BeginPlay()
{
	__super::BeginPlay();
	ControllPawn.push_back(static_cast<FPPawn*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "Cube")));

	ControllPawnSize = ControllPawn.size();
}

void GameController::Tick()
{
	__super::Tick();
}

void GameController::NextPawn(FInputValue value)
{
	ControllPawnIndex++;
	ControllPawnIndex %= ControllPawnSize;
	Possess(ControllPawn[ControllPawnIndex]);
}

void GameController::PrevPawn(FInputValue value)
{
	ControllPawnIndex--;
	if (ControllPawnIndex < 0)
	{
		ControllPawnIndex = ControllPawnSize - 1;
	}
	Possess(ControllPawn[ControllPawnIndex]);
}

void GameController::SetFillTriangel(FInputValue Value)
{	
	bisFill = !bisFill;
	std::cout << "Controller" << bisFill << "\n";
	ForestPearlEngine::GetGameEngine().SetbFill(bisFill);
	std::vector<FPActor*> ActorList;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "UI", ActorList);
	for (FPActor* Actor : ActorList)
	{
		UI* GameUI = static_cast<UI*>(Actor);
		GameUI->SetFill(Value);
	}
	ActorList.clear();
}

void GameController::SetCullTriangle(FInputValue Value)
{
	bisCull = !bisCull;
	ForestPearlEngine::GetGameEngine().SetbCull(bisCull);
	std::vector<FPActor*> ActorList;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "UI", ActorList);
	for (FPActor* Actor : ActorList)
	{
		UI* GameUI = static_cast<UI*>(Actor);
		GameUI->SetCull(Value);
	}
	ActorList.clear();
}

void GameController::SetActiveViewHelp(struct FInputValue Value)
{
	std::vector<FPActor*> ActorList;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "UI", ActorList);
	for (FPActor* Actor : ActorList)
	{
		UI* GameUI = static_cast<UI*>(Actor);
		GameUI->SetActiveViewHelp(Value);
	}
	ActorList.clear();
}

void GameController::SetActiveDepthStencilBuffer(struct FInputValue Value)
{
	std::vector<FPActor*> ActorList;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "UI", ActorList);
	for (FPActor* Actor : ActorList)
	{
		UI* GameUI = static_cast<UI*>(Actor);
		GameUI->SetActiveDepthStencilBuffer(Value);
	}
	ActorList.clear();
}

void GameController::SetGridOn(struct FInputValue Value)
{
	std::vector<FPActor*> ActorList;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "Grid", ActorList);
	for (FPActor* Actor : ActorList)
	{
		Grid* GameGrid = static_cast<Grid*>(Actor);
		GameGrid->SetActiveViewHelp(Value);
	}
	ActorList.clear();

	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "UI", ActorList);
	for (FPActor* Actor : ActorList)
	{
		UI* GameUI = static_cast<UI*>(Actor);
		GameUI->SetGridOn(Value);
	}
	ActorList.clear();
}

void GameController::SetAxisOn(struct FInputValue Value)
{
	std::vector<FPActor*> ActorList;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "Axis", ActorList);
	for (FPActor* Actor : ActorList)
	{
		Axis* GameAxis = static_cast<Axis*>(Actor);
		GameAxis->SetActiveViewHelp(Value);
	}
	ActorList.clear();

	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "UI", ActorList);
	for (FPActor* Actor : ActorList)
	{
		UI* GameUI = static_cast<UI*>(Actor);
		GameUI->SetAxisOn(Value);
	}
	ActorList.clear();
}

void GameController::SetNormalLine(FInputValue Value)
{
	bNormal = !bNormal;
	ForestPearlEngine::GetGameEngine().SetbNormal(bNormal);
	std::vector<FPActor*> ActorList;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "UI", ActorList);
	for (FPActor* Actor : ActorList)
	{
		UI* GameUI = static_cast<UI*>(Actor);
		GameUI->SetNormalLine(Value);
	}
	ActorList.clear();
}