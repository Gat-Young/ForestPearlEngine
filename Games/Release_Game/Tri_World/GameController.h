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
		
		bool bisFill = true;
		bool bisCull = true;

	public :
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void NextPawn(FInputValue value);
		void PrevPawn(FInputValue value);

		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);
		void SetActiveViewHelp(struct FInputValue Value);
		void SetActiveDepthStencilBuffer(struct FInputValue Value);
		void SetGridOn(struct FInputValue Value);
		void SetAxisOn(struct FInputValue Value);
};