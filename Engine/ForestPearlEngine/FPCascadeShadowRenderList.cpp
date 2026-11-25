#include "FPCascadeShadowRenderList.h"

FPCascadeShadowRenderItem* FPCascadeShadowRenderList::RegistRenderList()
{
	RenderList.push_back(FPCascadeShadowRenderItem{});

	return &(RenderList.back());
}

void FPCascadeShadowRenderList::UnregistRenderList(FPCascadeShadowRenderItem* Renderitem)
{
	auto it = std::find_if(RenderList.begin(), RenderList.end(),
		[Renderitem](FPCascadeShadowRenderItem& item)
		{
			return &item == Renderitem;
		});

	if (it != RenderList.end())
	{
		RenderList.erase(it);
	}
}