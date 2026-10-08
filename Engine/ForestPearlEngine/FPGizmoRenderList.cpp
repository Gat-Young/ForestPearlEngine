#include "FPGizmoRenderList.h"

GizmoRenderItem* FPGizmoRenderList::RegistRenderList()
{
	RenderList.push_back(GizmoRenderItem{});

	return &(RenderList.back());
}

void FPGizmoRenderList::UnregistRenderList(GizmoRenderItem* Renderitem)
{
	auto it = std::find_if(RenderList.begin(), RenderList.end(),
		[Renderitem](GizmoRenderItem& item)
		{
			return &item == Renderitem;
		});

	if (it != RenderList.end())
	{
		RenderList.erase(it);
	}
}