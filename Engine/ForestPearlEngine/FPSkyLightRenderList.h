#pragma once
#include "Renderers/FPRenderingCommon.h"
#include "FPGameInstanceSubSystem.h"

class FPSkyLightRenderList : public FPGameInstanceSubSystem
{
private:
	std::vector<FPSkyLightRenderItem> RenderList;

public:

	FPSkyLightRenderList() = default;
	~FPSkyLightRenderList() = default;

	FPSkyLightRenderItem* RegistRenderList();
	void UnregistRenderList(FPSkyLightRenderItem* RenderItem);

	std::vector<FPSkyLightRenderItem>& GetRenderList()
	{
		return RenderList;
	}

};
