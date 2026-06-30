#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"
#include "../../Engine/ForestPearlEngine/GizmoComponent.h"
#include "../../Engine/ForestPearlEngine/Systems/KeyStateEnum.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"


class Grid : public FPActor
{
	private:
		FPVector3 Position{0.0f, 0.0f, 0.0f};
		FPVector3 Rotation{ 0.0f, 0.0f, 0.0f };
		FPVector3 Scale{ 1.0f, 1.0f, 1.0f };

		GizmoComponent* GridComponets;
		bool bShow = true;

	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
		void SetActiveViewHelp(struct FInputValue Value);
};