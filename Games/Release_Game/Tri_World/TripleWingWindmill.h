#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;

class TripleWingWindmill : public FPPawn
{
private:
	FPStaticMeshComponent* Body;
	FPStaticMeshComponent* Wing1;
	FPStaticMeshComponent* Wing2;
	FPStaticMeshComponent* Wing3;

	float ScaleOffset = 1.0f;

	float Wing1Speed = 30.0f;
	float Wing2Speed = 180.0f;
	float Wing3Speed = 360.0f;

public:
	TripleWingWindmill() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void Move(FInputValue Value);
	void Rotate(FInputValue Value);
	void Scaling(FInputValue Value);

};