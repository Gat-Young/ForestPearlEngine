#pragma once
#include "../ForestPearlEngine/Object/Actor.h"

class FPAController : public FPActor
{
	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		virtual ~FPAController() = default;
};