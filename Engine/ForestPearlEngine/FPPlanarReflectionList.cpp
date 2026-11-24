#include "FPPlanarReflectionList.h"
#include <algorithm>

PlanarReflectionItem* FPPlanarReflectionList::RegistRenderList()
{
	PlanarReflectionRenderList.push_back(PlanarReflectionItem{});

	return &(PlanarReflectionRenderList.back());
}

void FPPlanarReflectionList::UnregistRenderList(PlanarReflectionItem* RenderItem)
{
	auto it = std::find_if(PlanarReflectionRenderList.begin(), PlanarReflectionRenderList.end(),
		[RenderItem](PlanarReflectionItem& item)
		{
			return &item == RenderItem;
		});

	if (it != PlanarReflectionRenderList.end())
	{
		PlanarReflectionRenderList.erase(it);
	}
}
