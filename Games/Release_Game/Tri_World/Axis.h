#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class GizmoAxisComponent;

class Axis : public FPActor
{
private:

	GizmoAxisComponent* AxisComponets;
	bool bShow = true;

public:
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
	void SetActiveViewHelp(struct FInputValue Value);
};