#pragma once
#include <vector>
#include "ForestPearlEngine/Object/FPPawn.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;
class FPSpringArmComponent;
class FPCameraComponent;

class AShip : public FPPawn
{
	private:
		std::vector<FPStaticMeshComponent*> ShipParts;
		FPSpringArmComponent* SpringArm;
		FPCameraComponent* PlayerCamera;

	public:
		AShip() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void CameraMove(FInputValue Value);
};
