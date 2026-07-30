#pragma once
#include "Renderers/FPRenderingCommon.h"
#include "FPGameInstanceSubSystem.h"

class FPMeshRenderList : public FPGameInstanceSubSystem
{
	private:
		std::vector<RenderItem> RenderList;

	public:

		FPMeshRenderList() = default;
		~FPMeshRenderList() = default;

		RenderItem* RegistRenderList();
		void UnregistRenderList(RenderItem* RenderItem);

		std::vector<RenderItem>& GetRenderList()
		{
			return RenderList;
		}

};