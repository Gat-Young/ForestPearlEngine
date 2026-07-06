#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"
#include "../../Engine/ForestPearlEngine/CameraComponent.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;

class GameCamera : public FPActor
{
private:
	CameraComponent* Camera;

public:
	GameCamera() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
};