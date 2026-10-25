#pragma once
#include "FPLightComponent.h"

class FPDirectionalLightComponent : public FPLightComponent
{
	protected:

	public:
		FPDirectionalLightComponent(FPActor* Ownder);
		~FPDirectionalLightComponent();
};
