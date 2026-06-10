#pragma once
#include "TextRenderList.h"

struct UIContext
{
	int x;
	int y;
	unsigned long color;
	std::basic_string<TCHAR> msg;
};

class TextComponent
{
	private:
		int x;
		int y;
		unsigned long color;
		std::basic_string<TCHAR> msg;
		UIContextItem* RenderItem = nullptr;

		void RegistMeshRenderList();

	public:
		TextComponent();

		void SetTextData(int x, int y, unsigned long color, std::basic_string<TCHAR> msg);
		~TextComponent();
};