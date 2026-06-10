#pragma once
#include "../../../../Engine/ForestPearlEngine/Object/Actor.h"
#include "../../../../Engine/ForestPearlEngine/Systems/KeyStateEnum.h"
#include "../../../../Engine/ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;

class Player : public FPActor
{
	public :
		Player();
		~Player();
		virtual void BeginPlay() override;
		virtual void Tick() override;

	private:
		FPInputMappingContext* IMC = nullptr;
		FPInputAction* IA = nullptr;

		void Move(FPVector2 Value);
};
