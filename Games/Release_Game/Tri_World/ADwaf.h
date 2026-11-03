#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;
class FPSpringArmComponent;
class FPCameraComponent;

class ADwaf : public FPPawn
{
private:

	//Dwaf Model
	FPStaticMeshComponent* Head;
	FPStaticMeshComponent* Body;
	FPStaticMeshComponent* Beard;
	FPStaticMeshComponent* Armor;
	FPStaticMeshComponent* Pack;
	FPStaticMeshComponent* Sword1;

	float angle = 0;

	float AngleSpeed = 0.25f;

public:
	ADwaf() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void Move(FInputValue value);
	void Rotate(FInputValue value);
	void CameraMove(FInputValue value);
	void CameraZoom(FInputValue Value);
	void Scaling(FInputValue Value);
};