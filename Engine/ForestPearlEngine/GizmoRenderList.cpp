#include "GizmoRenderList.h"

GizmoRenderItem* GizmoRenderList::RegistRenderList()
{
	RenderList.push_back(GizmoRenderItem{});

	return &(RenderList.back());
}

void GizmoRenderList::UnregistRenderList(GizmoRenderItem* RenderItem)
{
	auto it = std::find_if(RenderList.begin(), RenderList.end(),
		[RenderItem](GizmoRenderItem& item)
		{
			return &item == RenderItem;
		});

	if (it != RenderList.end())
	{
		RenderList.erase(it);
	}
}