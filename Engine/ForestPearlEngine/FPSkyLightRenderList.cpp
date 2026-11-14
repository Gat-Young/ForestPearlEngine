#include "FPSkyLightRenderList.h"

FPSkyLightRenderItem* FPSkyLightRenderList::RegistRenderList()
{
	RenderList.push_back(FPSkyLightRenderItem{});

	return &(RenderList.back());
}

void FPSkyLightRenderList::UnregistRenderList(FPSkyLightRenderItem* Renderitem)
{
	auto it = std::find_if(RenderList.begin(), RenderList.end(),
		[Renderitem](FPSkyLightRenderItem& item)
		{
			return &item == Renderitem;
		});

	if (it != RenderList.end())
	{
		RenderList.erase(it);
	}
}
