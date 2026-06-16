#pragma once
#include "../../Engine/ForestPearlEngine/FPWorld.h"

class GameWorld : public FPWorld
{
	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		FPAGameMode& GetGameMode() { return *GameMode; }
};