#pragma once
#include <string>
#include "tchar.h"
#include <vector>
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"

struct UIContextItem
{
	bool** active;
	int* x;
	int* y;
	FPVector4* color;
	std::basic_string<TCHAR>* msg;
};

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