#include "FPTextRenderList.h"

UIContextItem* FPTextRenderList::RegistRenderList()
{
	RenderList.push_back(UIContextItem{});

	return &(RenderList.back());
}

void FPTextRenderList::UnregistRenderList(UIContextItem* RenderItem)
{
	auto it = std::find_if(RenderList.begin(), RenderList.end(),
		[RenderItem](UIContextItem& item)
		{
			return &item == RenderItem;
		});

	if (it != RenderList.end())
	{
		RenderList.erase(it);
	}
}