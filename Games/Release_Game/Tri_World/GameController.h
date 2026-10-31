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
		bool bNormal = false;

		int SeasonCnt = 0;

	public :
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void NextPawn(FInputValue value);
		void PrevPawn(FInputValue value);

		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);
		void SetNormalLine(FInputValue Value);

		void SetActiveViewHelp(FInputValue Value);
		void SetActiveDepthStencilBuffer(FInputValue Value);
		void SetGridOn(FInputValue Value);
		void SetAxisOn(FInputValue Value);

		void SetDirectionalLight(FInputValue Value);
		void OnTorch(FInputValue Value);

		void ChangeSeason(FInputValue Value);
};