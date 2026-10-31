#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;

class WindmillWing : public FPPawn
{
private:
	FPStaticMeshComponent* Wing;
	FPActor* Body;
	FPActor* Player;
	FPSceneComponent* ShieldPivot;

	float ScaleOffset = 1.0f;

	bool isHead = false;
	bool isShield = false;

	float WingSpeed = 180.0f;

	float SpeedOffset = 1.0f;
	float deltaSpeed = 0.05f;

	bool isWinter = false;

public:
	WindmillWing() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void AttachHead(FInputValue Value);
	void AttachShield(FInputValue Value);
	void SetScaleWing(FInputValue Value);

	void ChangeSeason(int SeasonCnt);
};