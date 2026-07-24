#include "MeshRenderList.h"

RenderItem* MeshRenderList::RegistRenderList()
{
	RenderList.push_back(RenderItem{});

	return &(RenderList.back());
}

void MeshRenderList::UnregistRenderList(RenderItem* Renderitem)
{
	auto it = std::find_if(RenderList.begin(), RenderList.end(), 
		[Renderitem](RenderItem& item)
		{
			return &item == Renderitem;
		});

	if (it != RenderList.end())
	{
		RenderList.erase(it);
	}
}