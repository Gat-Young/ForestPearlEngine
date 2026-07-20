#include "GameCamera.h"
#include "ForestPearlEngine/CameraComponent.h"
#include <iostream>

void GameCamera::Initialize()
{
	Camera = new CameraComponent(this);
	SetRootComponent((FPSceneComponent*)Camera);

	SetActorLocation({0.0f, 20.0f, -45.0f});
}

void GameCamera::BeginPlay()
{
}

void GameCamera::Tick()
{
}