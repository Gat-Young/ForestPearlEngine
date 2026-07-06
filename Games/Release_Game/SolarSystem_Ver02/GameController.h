#pragma once
#include "../../../Engine/ForestPearlEngine/FPAController.h"

class GameController : public FPAController
{
	public :
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

	private:
		void OnMouseLButtonDown(struct FInputValue InputValue);
		void OnMouseRButtonDown(struct FInputValue InputValue);
};