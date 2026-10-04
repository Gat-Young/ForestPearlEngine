#pragma once
#include "Object/Actor.h"
#include "Define/FPMath.h"

class FPInputComponent;
class FPPawn;

class FPAController : public FPActor
{
	public:
		FPAController();
		~FPAController();

		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		//Possess 관련
		void Possess(FPPawn* PossessedPawn);
		void UnPossess();
		FPPawn* GetPawn();						//Possess된 Pawn 정보를 반환

		//Controller 회전
		void AddYawInput(float Value);
		void AddPitchInput(float Value);
		void AddRollInput(float Value);

		FPInputComponent& GetInputComponent() { return *InputComponent; }

	private:
		FPInputComponent* InputComponent;
		FPPawn* PossessedPawn = nullptr;
		//회전 누적용
		FPVector3 ControlRotation;
};