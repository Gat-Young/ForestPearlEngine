#include "FPTextComponent.h"
#include "FPGameInstance.h"

FPTextComponent::FPTextComponent()
{
	RegistTextRenderList();
}

void FPTextComponent::RegistTextRenderList()
{
	FPTextRenderList* TextRenderList = static_cast<FPTextRenderList*>(FPGameInstance::Get().GetTextRenderList());
	RenderItem = TextRenderList->RegistRenderList();

	RenderItem->active = &(this->active);
	RenderItem->x = &(this->x);
	RenderItem->y = &(this->y);
	RenderItem->color = &(this->color);
	RenderItem->msg = &(this->msg);
}

void FPTextComponent::SetTextData(bool* active, int x, int y, FPVector4 color, std::basic_string<TCHAR> msg)
{
	this->active = active;
	this->x = x;
	this->y = y;
	this->color = color;
	this->msg = msg;
}

FPTextComponent::~FPTextComponent()
{
	FPTextRenderList* TextRenderList = static_cast<FPTextRenderList*>(FPGameInstance::Get().GetTextRenderList());
	TextRenderList->UnregistRenderList(RenderItem);
}