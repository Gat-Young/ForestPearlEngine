#pragma once
#include "../ForestPearlEngine/Object/Actor.h"
#include "FPAController.h"
#include <string>
#include <vector>


class FPAGameMode : public FPActor
{
	protected:
		//0¹øÀº Player Controller;
		std::vector<std::string> ControllerList;
		std::vector<FPAController*> GameController;

	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		virtual ~FPAGameMode() = default;

		FPAController* GetController(int index) { return GameController[index]; }
};