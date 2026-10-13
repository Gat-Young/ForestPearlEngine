#include "FPMeshNormalRenderList.h"

NormalRenderItem* FPMeshNormalRenderList::RegistRenderList()
{
	RenderList.push_back(NormalRenderItem{});

	return &(RenderList.back());
}

void FPMeshNormalRenderList::UnregistRenderList(NormalRenderItem* Renderitem)
{
	auto it = std::find_if(RenderList.begin(), RenderList.end(),
		[Renderitem](NormalRenderItem& item)
		{
			return &item == Renderitem;
		});

	if (it != RenderList.end())
	{
		RenderList.erase(it);
	}
}