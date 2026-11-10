#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"

class FPStaticMeshComponent;
class FPSceneCaptureComponent2D;
class FPSpringArmComponent;
struct FInputValue;

class AMirror : public FPPawn
{
private:
	FPStaticMeshComponent* MirrorPlane;
	FPSpringArmComponent* SpringArm;
	FPSceneCaptureComponent2D* ScreenCaptureComponent;

public:
	AMirror() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void CameraMove(FInputValue Value);
	void CameraZoom(FInputValue Value);
};
