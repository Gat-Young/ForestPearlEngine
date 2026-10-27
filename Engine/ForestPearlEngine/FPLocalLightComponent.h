#pragma once
#include "FPLightComponent.h"

class FPLocalLightComponent : public FPLightComponent
{
	public:
		FPLocalLightComponent(FPActor* Owner);
		~FPLocalLightComponent();
};