#pragma once
#include "FPLightComponentBase.h"

class FPLightComponent : public FPLightComponentBase
{
	protected:
		virtual void RegistLightRenderList() override;

	public:
		FPLightComponent(FPActor* Owner);
		~FPLightComponent();
};