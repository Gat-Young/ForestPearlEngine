#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;

class TripleWindmillWing : public FPPawn
{
private:
	FPStaticMeshComponent* Wing;
	FPStaticMeshComponent* Wing1;
	FPStaticMeshComponent* Wing2;
	FPActor* Body;

	float ScaleOffset = 1.0f;

	bool isHead = false;
	bool isShield = false;


	float Wing1Speed = 180.0f;
	float Wing2Speed = -360.0f;
	float Wing3Speed = 360.0f;

	float SpeedOffset = 1.0f;
	float deltaSpeed = 0.05f;


	bool isWinter = false;

public:
	TripleWindmillWing() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void SetScaleWing(FInputValue Value);
	void ChangeSeason(int SeasonCnt);
};