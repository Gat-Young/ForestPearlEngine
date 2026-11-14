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

class AUtaTeapot : public FPPawn
{
	private:
		FPStaticMeshComponent* TeapotBottom;
		FPStaticMeshComponent* TeapotUp;
		FPSpringArmComponent* SpringArm;
		FPCameraComponent* PlayerCamera;

	public:
		AUtaTeapot() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FInputValue Value);
		void Rotate(FInputValue Value);
		void CameraMove(FInputValue Value);
};
