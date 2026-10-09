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


class Cube : public FPPawn
{
	private:
		FPStaticMeshComponent* Mesh;
		FPSpringArmComponent* SpringArm;
		FPCameraComponent* PlayerCamera;


		float angle = 0;

		float AngleSpeed = 0.25f;

	public:
		Cube() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FInputValue value);
		void CameraMove(FInputValue value);
		void ScaleUp(FInputValue Value);
};