#pragma once
#include "Renderers/FPRenderingCommon.h"
#include "FPGameInstanceSubSystem.h"

class FPMeshNormalRenderList : public FPGameInstanceSubSystem
{
private:
	std::vector<NormalRenderItem> RenderList;

public:

	FPMeshNormalRenderList() = default;
	~FPMeshNormalRenderList() = default;

	NormalRenderItem* RegistRenderList();
	void UnregistRenderList(NormalRenderItem* RenderItem);

	std::vector<NormalRenderItem>& GetRenderList()
	{
		return RenderList;
	}

};