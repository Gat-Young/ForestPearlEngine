#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPMeshComponent;

class TripleWindmillWing : public FPPawn
{
private:
	FPMeshComponent* Wing;
	FPMeshComponent* Wing1;
	FPMeshComponent* Wing2;
	FPActor* Body;

	float ScaleOffset = 1.0f;
	bool isFill = true;
	bool isCull = false;

	bool isHead = false;
	bool isShield = false;

public:
	TripleWindmillWing() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void SetScaleWing(FInputValue Value);
	void SetFillTriangel(FInputValue Value);
	void SetCullTriangle(FInputValue Value);
};