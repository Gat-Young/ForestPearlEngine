#pragma once
#include "ForestPearlEngine/FPAController.h"

class GameController : public FPAController
{
	public :
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
};