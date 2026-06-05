#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"

class Triangle : public FPActor
{
public:
	Triangle();
	virtual void BeginPlay() override;
	virtual void Tick() override;
};