#include "GameCamera_Three.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPGameProjectSetting.h"
#include <iostream>

void GameCamera_Three::Initialize()
{
	Camera = new FPCameraComponent(this);

	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation({ 0.0f, 0.0f, -3.0f });

	FPGameProjectSetting* GameProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());

	Camera->SetViewPortSetting(GameProjectSetting->GetWinWidth() / 3 * 2, 0.0f,
								GameProjectSetting->GetWinWidth() / 3,
								GameProjectSetting->GetWinHeight(),
								0.0f, 1.0f);

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;


}

void GameCamera_Three::BeginPlay()
{
}

void GameCamera_Three::Tick()
{

	__super::Tick();
}