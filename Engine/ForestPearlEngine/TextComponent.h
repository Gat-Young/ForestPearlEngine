#pragma once
#include "TextRenderList.h"

struct UIContext
{
	bool active;
	int x;
	int y;
	unsigned long color;
	std::basic_string<TCHAR> msg;
};

class TextComponent
{
	private:
		bool* active;
		int x;
		int y;
		unsigned long color;
		std::basic_string<TCHAR> msg;
		UIContextItem* RenderItem = nullptr;

		void RegistTextRenderList();

	public:
		TextComponent();

		void SetTextData(bool* active, int x, int y, unsigned long color, std::basic_string<TCHAR> msg);
		~TextComponent();
};