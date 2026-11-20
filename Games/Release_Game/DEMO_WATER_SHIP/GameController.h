#pragma once
#include "ForestPearlEngine/FPAController.h"

struct FInputValue;

class GameController : public FPAController
{
	public :
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		// Enter: fire every Cannon. Enter+1/2/3: Frigate broadside (All/Left/Right).
		void FireCannon(FInputValue Value);
};
