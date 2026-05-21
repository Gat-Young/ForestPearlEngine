#pragma once
#include "../../../../Engine/ForestPearlEngine/Object/Actor.h"

class Player : public FPActor
{
	public :
		virtual void BeginPlay() override;
		virtual void Tick() override;
};
