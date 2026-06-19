#pragma once
#include "FPActorComponent.h"

class FPMovementComponent : public FPActorComponent
{
	protected:
		class FPSceneComponent* UpdatedComponent;

	public :
		FPMovementComponent(FPActor* Owner);
		void SetUpdatedComponent(class FPSceneComponent* Compoenet);

};