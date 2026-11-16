#include "FPSceneCaptureList.h"
#include <algorithm>

SceneCaptureItem* FPSceneCaptureList::RegistRenderList()
{
	SceneCaputreList.push_back(SceneCaptureItem{});

	return &(SceneCaputreList.back());
}

void FPSceneCaptureList::UnregistRenderList(SceneCaptureItem* RenderItem)
{
	auto it = std::find_if(SceneCaputreList.begin(), SceneCaputreList.end(),
		[RenderItem](SceneCaptureItem& item)
		{
			return &item == RenderItem;
		});

	if (it != SceneCaputreList.end())
	{
		SceneCaputreList.erase(it);
	}
}