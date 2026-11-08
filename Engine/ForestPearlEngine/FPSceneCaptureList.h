#pragma once
#include <vector>
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"
#include "../ForestPearlEngine/Renderers/FPRenderingCommon.h"

class FPSceneCaptureList : public FPGameInstanceSubSystem
{
private:
	std::vector<SceneCaptureItem> SceneCaputreList;

public:

	FPSceneCaptureList() = default;
	~FPSceneCaptureList() = default;

	SceneCaptureItem* RegistRenderList();
	void UnregistRenderList(SceneCaptureItem* RenderItem);

	std::vector<SceneCaptureItem>& GetRenderList()
	{
		return SceneCaputreList;
	}
};