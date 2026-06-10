#include "MeshRenderList.h"

MeshRenderItem* MeshRenderList::RegistRenderList()
{
	RenderList.push_back(MeshRenderItem{});

	return &(RenderList.back());
}

void MeshRenderList::UnregistRenderList(MeshRenderItem* RenderItem)
{
	auto it = std::find_if(RenderList.begin(), RenderList.end(), 
		[RenderItem](MeshRenderItem& item)
		{
			return &item == RenderItem;
		});

	if (it != RenderList.end())
	{
		RenderList.erase(it);
	}
}