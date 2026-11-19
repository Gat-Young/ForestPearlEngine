#pragma once
#include <vector>
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"
#include "../ForestPearlEngine/Renderers/FPRenderingCommon.h"

class FPPlanarReflectionList : public FPGameInstanceSubSystem
{
private:
	std::vector<PlanarReflectionItem> PlanarReflectionRenderList;

public:

	FPPlanarReflectionList() = default;
	~FPPlanarReflectionList() = default;

	PlanarReflectionItem* RegistRenderList();
	void UnregistRenderList(PlanarReflectionItem* RenderItem);

	std::vector<PlanarReflectionItem>& GetRenderList()
	{
		return PlanarReflectionRenderList;
	}
};
