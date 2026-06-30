#pragma once
#include <string>
#include "tchar.h"
#include <vector>

struct UIContextItem
{
	bool** active;
	int* x;
	int* y;
	unsigned long* color;
	std::basic_string<TCHAR>* msg;
};

class TextRenderList
{
	private:
		std::vector<UIContextItem> RenderList;

		TextRenderList() = default;
		~TextRenderList() = default;

	public:
		//Single Tone
		static TextRenderList& Get()
		{
			static TextRenderList Instance;
			return Instance;
		}

		UIContextItem* RegistRenderList();
		void UnregistRenderList(UIContextItem* RenderItem);

		std::vector<UIContextItem>& GetRenderList()
		{
			return RenderList;
		}
};