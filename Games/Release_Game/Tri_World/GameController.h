#pragma once
#include "ForestPearlEngine/FPAController.h"

#include <vector>

class FPPawn;
class FPInputMappingContext;
class FPInputAction;
struct FInputValue;

class GameController : public FPAController
{
	private:
		std::vector<FPPawn*> ControllPawn;
		int ControllPawnIndex = 0;
		int ControllPawnSize = 0;

	public :
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void NextPawn(FInputValue value);
		void PrevPawn(FInputValue value);
};