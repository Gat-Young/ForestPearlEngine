#pragma once
#include <string>
#include <vector>
#include "Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"
#include "Renderers/FPRenderingCommon.h"

class FPTextRenderList : public FPGameInstanceSubSystem
{
	private:
		std::vector<UIContextItem> RenderList;

	public:

		FPTextRenderList() = default;
		~FPTextRenderList() = default;

		UIContextItem* RegistRenderList();
		void UnregistRenderList(UIContextItem* RenderItem);

		std::vector<UIContextItem>& GetRenderList()
		{
			return RenderList;
		}
};