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

class Player : public FPPawn
{
	private:
		FPStaticMeshComponent* Mesh;
		FPSceneComponent* ShieldPivot;
		FPSpringArmComponent* SpringArm;
		FPCameraComponent* PlayerCamera;

		bool isFill = true;
		bool isCull = false;

		float angle = 0;

		float AngleSpeed = 0.25f;

	public:
		Player() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FInputValue value);
		void CameraMove(FInputValue value);
		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);
		FPSceneComponent* GetShieldPivot();
};