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
class WindmillWing;
class TripleWindmillWing;
class ATorchLight;

class Player : public FPPawn
{
	private:

		//ToonLink Model
		FPStaticMeshComponent* BodyAndHat;
		FPStaticMeshComponent* Buckle;
		FPStaticMeshComponent* Eye;
		FPStaticMeshComponent* EyeBrow;
		FPStaticMeshComponent* Face;
		FPStaticMeshComponent* Hair;
		FPStaticMeshComponent* Hand;
		FPStaticMeshComponent* LeftEyeBg;
		FPStaticMeshComponent* RightEyebg;
		FPStaticMeshComponent* MasterSword_Scabbard;
		FPStaticMeshComponent* MasterSword_Handle;
		FPStaticMeshComponent* MasterSword_Blade;
		FPStaticMeshComponent* MasterSword_BackHandle;
		FPStaticMeshComponent* Mouth;
		FPStaticMeshComponent* Shield_Back;
		FPStaticMeshComponent* Shidld_Hand;

		ATorchLight* TorchLight = nullptr;

		FPSceneComponent* ShieldPivot;
		FPSpringArmComponent* SpringArm;
		FPCameraComponent* PlayerCamera;
		WindmillWing* OneWindmillWing;
		TripleWindmillWing* TripleWing;

		float angle = 0;

		float AngleSpeed = 0.25f;

	public:
		Player() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void SetOneWindmillWing(WindmillWing* Wing);
		void SetTripleWindmillWing(TripleWindmillWing* Wing);

		void Move(FInputValue value);
		void CameraMove(FInputValue value);
		void CameraZoom(FInputValue Value);
		void SetScaleWing(FInputValue Value);
		void AttachHead(FInputValue Value);
		void AttachShield(FInputValue Value);

		void OnTorch(FInputValue Value);

		FPSceneComponent* GetShieldPivot();
};