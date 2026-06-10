#pragma once
#include "../../Engine/ForestPearlEngine/Object//Object.h"

class ADOHWAGameManager : public FPObject
{
	public:
		virtual void BeginPlay() override;
		virtual void Tick() override;
};