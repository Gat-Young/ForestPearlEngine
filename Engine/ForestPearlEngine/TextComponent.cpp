#include "TextComponent.h"

TextComponent::TextComponent()
{
	RegistTextRenderList();
}

void TextComponent::RegistTextRenderList()
{
	RenderItem = TextRenderList::Get().RegistRenderList();

	RenderItem->active = &(this->active);
	RenderItem->x = &(this->x);
	RenderItem->y = &(this->y);
	RenderItem->color = &(this->color);
	RenderItem->msg = &(this->msg);
}

void TextComponent::SetTextData(bool active, int x, int y, unsigned long color, std::basic_string<TCHAR> msg)
{
	this-> active = active;
	this->x = x;
	this->y = y;
	this->color = color;
	this->msg = msg;
}

TextComponent::~TextComponent()
{
	TextRenderList::Get().UnregistRenderList(RenderItem);
}