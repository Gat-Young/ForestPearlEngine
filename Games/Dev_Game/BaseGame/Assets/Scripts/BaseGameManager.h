#pragma once
#include "../../../../Engine/ForestPearlEngine/Object/Object.h"

class ABaseGameManager : public FPObject
{
	public:
		virtual void BeginPlay() override;
		virtual void Tick() override;
};