#pragma once
#include <vector>
#include "ForestPearlEngine/Object/FPPawn.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;
class FPSpringArmComponent;
class FPCameraComponent;

class ACar : public FPPawn
{
	private:
		std::vector<FPStaticMeshComponent*> CarParts;
		FPSpringArmComponent* SpringArm;
		FPCameraComponent* PlayerCamera;

	public:
		ACar() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void CameraMove(FInputValue Value);
		void CameraZoom(FInputValue Value);
};
