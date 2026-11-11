#pragma once
#include "ForestPearlEngine/FPAGameMode.h"

class GameMode : public FPAGameMode
{
	public :
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		virtual ~GameMode() = default;
};