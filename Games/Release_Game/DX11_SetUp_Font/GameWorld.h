#pragma once
#include "ForestPearlEngine/FPWorld.h"

class GameWorld : public FPWorld
{
	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
};