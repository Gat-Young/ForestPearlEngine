#pragma once
#include "../ForestPearlEngine/Object/FPPawn.h"

class FPInputComponent;

class FPAController : public FPActor
{
	public:
		FPAController();
		~FPAController();

		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Possess(FPPawn* PossessedPawn);
		void UnPossess();

		FPInputComponent& GetInputComponent() { return *InputComponent; }

	private:
		FPInputComponent* InputComponent;
		FPPawn* PossessedPawn;
};