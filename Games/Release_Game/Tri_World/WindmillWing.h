#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPMeshComponent;

class WindmillWing : public FPPawn
{
private:
	FPMeshComponent* Wing;
	FPActor* Body;
	FPActor* Player;
	FPSceneComponent* ShieldPivot;

	float ScaleOffset = 1.0f;
	bool isFill = true;
	bool isCull = false;

	bool isHead = false;
	bool isShield = false;

public:
	WindmillWing() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void AttachHead(FInputValue Value);
	void AttachShield(FInputValue Value);
	void SetScaleWing(FInputValue Value);
	void SetFillTriangel(FInputValue Value);
	void SetCullTriangle(FInputValue Value);
};