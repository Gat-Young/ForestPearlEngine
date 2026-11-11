#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"
#include <vector>

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;
class FPSpringArmComponent;
class FPCameraComponent;


class Ship : public FPPawn
{
	private:

		//JosenShip Model
		std::vector<FPStaticMeshComponent*> JosenShipMeshes;


		FPSceneComponent* ShieldPivot;
		FPSpringArmComponent* SpringArm;
		FPCameraComponent* PlayerCamera;


		float angle = 0;

		float AngleSpeed = 0.25f;

	public:
		Ship() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;


		void Move(FInputValue value);
		void CameraMove(FInputValue value);
		void CameraZoom(FInputValue Value);

};