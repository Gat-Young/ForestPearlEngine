#pragma once
#include "../../Engine/ForestPearlEngine/Object/Object.h"

class AGameManager : public FPObject
{
	public:
		virtual void BeginPlay() override;
		virtual void Tick() override;
};