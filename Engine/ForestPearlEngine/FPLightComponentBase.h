#pragma once
#include "FPSceneComponent.h"
#include "FPLightRenderList.h"

class FPLightComponentBase : public FPSceneComponent
{
	protected:
		bool isActive = true;
		FPLightRenderItem* RenderItem = nullptr;

		virtual void RegistLightRenderList() = 0;

	public:
		FPLightComponentBase(FPActor* Owner);
		~FPLightComponentBase();
		void SetActive(bool Active) { this->isActive = Active; }
};