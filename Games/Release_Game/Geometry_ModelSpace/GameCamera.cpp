#include "GameCamera.h"
#include "ForestPearlEngine/CameraComponent.h"
#include <iostream>

void GameCamera::Initialize()
{
	Camera = new CameraComponent(this);
	SetRootComponent((FPSceneComponent*)Camera);

	SetActorLocation({0.0f, 3.0f, -10.0f});
	std::cout << "camera Create" << "\n";
}

void GameCamera::BeginPlay()
{
}

void GameCamera::Tick()
{
}