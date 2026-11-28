#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

struct FInputValue;
class FPStaticMeshComponent;
class FPSpringArmComponent;
class FPCameraComponent;

// Third-person free camera pawn: WASD move, IJKL camera rotate, mouse wheel zoom.
class Player : public FPPawn
{
	private:
		FPStaticMeshComponent* BodyMesh;
		FPSpringArmComponent* SpringArm;
		FPCameraComponent* PlayerCamera;

	public:
		Player() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FInputValue Value);
		void CameraMove(FInputValue Value);
		void CameraZoom(FInputValue Value);
};
