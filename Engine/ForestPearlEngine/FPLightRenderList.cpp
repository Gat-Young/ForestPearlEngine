#include "FPLightRenderList.h"

FPLightRenderItem* FPLightRenderList::RegistRenderList()
{
	RenderList.push_back(FPLightRenderItem{});

	return &(RenderList.back());
}

void FPLightRenderList::UnregistRenderList(FPLightRenderItem* Renderitem)
{
	auto it = std::find_if(RenderList.begin(), RenderList.end(),
		[Renderitem](FPLightRenderItem& item)
		{
			return &item == Renderitem;
		});

	if (it != RenderList.end())
	{
		RenderList.erase(it);
	}
}