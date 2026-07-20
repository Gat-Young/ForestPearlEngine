#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class CameraComponent;

class GameCamera : public FPActor
{
private:
	CameraComponent* Camera;
	FPSceneComponent* Target;

public:
	GameCamera() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void Move(FInputValue value);
};