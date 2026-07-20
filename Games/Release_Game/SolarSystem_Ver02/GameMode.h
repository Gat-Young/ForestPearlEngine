#pragma once
#include "../../../Engine/ForestPearlEngine/FPAGameMode.h"

class GameMode : public FPAGameMode
{
	public :
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		virtual ~GameMode() = default;

		void AddOrb(struct FPVector2 SpawnPos);
};