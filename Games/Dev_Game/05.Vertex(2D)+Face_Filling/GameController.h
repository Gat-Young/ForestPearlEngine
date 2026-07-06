#pragma once
#include "../../Engine/ForestPearlEngine/FPAController.h"

class FPInputMappingContext;

class GameController : public FPAController
{
	public :
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

private:
};