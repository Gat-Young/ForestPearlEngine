#pragma once
#include "Renderers/FPRenderingCommon.h"
#include "FPGameInstanceSubSystem.h"

class FPCascadeShadowRenderList : public FPGameInstanceSubSystem
{
private:
	std::vector<FPCascadeShadowRenderItem> RenderList;

public:

	FPCascadeShadowRenderList() = default;
	~FPCascadeShadowRenderList() = default;

	FPCascadeShadowRenderItem* RegistRenderList();
	void UnregistRenderList(FPCascadeShadowRenderItem* RenderItem);

	std::vector<FPCascadeShadowRenderItem>& GetRenderList()
	{
		return RenderList;
	}

};