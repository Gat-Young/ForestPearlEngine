#pragma once
#include "Renderers/FPRenderingCommon.h"
#include "FPGameInstanceSubSystem.h"

class FPGizmoRenderList : public FPGameInstanceSubSystem
{
private:
	std::vector<GizmoRenderItem> RenderList;

public:

	FPGizmoRenderList() = default;
	~FPGizmoRenderList() = default;

	GizmoRenderItem* RegistRenderList();
	void UnregistRenderList(GizmoRenderItem* RenderItem);

	std::vector<GizmoRenderItem>& GetRenderList()
	{
		return RenderList;
	}

};