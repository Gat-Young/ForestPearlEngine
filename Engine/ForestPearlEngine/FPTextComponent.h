#pragma once
#include "FPTextRenderList.h"

class FPTextComponent
{
	private:
		bool* active;
		int x;
		int y;
		FPVector4 color;
		std::basic_string<TCHAR> msg;
		UIContextItem* RenderItem = nullptr;

		void RegistTextRenderList();

	public:
		FPTextComponent();

		void SetTextData(bool* active, int x, int y, FPVector4 color, std::basic_string<TCHAR> msg);
		~FPTextComponent();
};