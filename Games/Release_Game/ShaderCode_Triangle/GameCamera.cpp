#include "GameCamera.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPGameProjectSetting.h"
#include <iostream>

void GameCamera::Initialize()
{
	Camera = new FPCameraComponent(this);

	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation({ 0.0f, 0.0f, -3.0f });

	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());

	Camera->SetViewPortSetting(0.0f, 100.0f,
								GameProjectSetting->GetWinWidth()/3,
								400,
								0.0f,1.0f);

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;


}

void GameCamera::BeginPlay()
{
}

void GameCamera::Tick()
{

	__super::Tick();
}