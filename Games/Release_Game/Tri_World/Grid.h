#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class GizmoGridComponent;

class Grid : public FPActor
{
private:

	GizmoGridComponent* GridComponets;
	bool bShow = true;

public:
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
	void SetActiveViewHelp(struct FInputValue Value);
};