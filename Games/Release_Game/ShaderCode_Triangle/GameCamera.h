#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPCameraComponent;

class GameCamera : public FPActor
{
private:
	FPCameraComponent* Camera;
	FPSceneComponent* Target;
	//카메라 회전 누적값
	FPQuaternion CameraRotation;

public:
	GameCamera() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

};