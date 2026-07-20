#pragma once
#include "TextRenderList.h"

class TextComponent
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
		TextComponent();

		void SetTextData(bool* active, int x, int y, FPVector4 color, std::basic_string<TCHAR> msg);
		~TextComponent();
};