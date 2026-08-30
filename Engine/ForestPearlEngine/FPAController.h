#pragma once
#include "../ForestPearlEngine/Object/Actor.h"

class FPInputComponent;

class FPAController : public FPActor
{
	public:
		FPAController();
		~FPAController();

		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Possess(FPActor* PossessActor);
		void UnPossess();

		FPInputComponent& GetInputComponent() { return *InputComponent; }

	private:
		FPInputComponent* InputComponent;
		FPActor* PossesedActor;
};