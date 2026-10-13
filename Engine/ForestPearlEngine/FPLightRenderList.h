#pragma once
#include "Renderers/FPRenderingCommon.h"
#include "FPGameInstanceSubSystem.h"

class FPLightRenderList : public FPGameInstanceSubSystem
{
private:
	std::vector<FPLightRenderItem> RenderList;

public:

	FPLightRenderList() = default;
	~FPLightRenderList() = default;

	FPLightRenderItem* RegistRenderList();
	void UnregistRenderList(FPLightRenderItem* RenderItem);

	std::vector<FPLightRenderItem>& GetRenderList()
	{
		return RenderList;
	}

};