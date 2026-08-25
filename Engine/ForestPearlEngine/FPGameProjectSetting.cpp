#include "FPGameProjectSetting.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include "Utility/FPPathManager.h"

void FPGameProjectSetting::CalculateDisplaySize()
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	DisplayWidth = rect.right - rect.left;
	DisplayHeight = rect.bottom - rect.top;
}

void FPGameProjectSetting::SetHWND(HWND hwnd)
{
	this->hwnd = hwnd;
}

void FPGameProjectSetting::ProjectSetting()
{
	WinClassName = FPPathManager::Get().StringToWString(FPPathManager::Get().GetProjectName());
	WinName = FPPathManager::Get().StringToWString(FPPathManager::Get().GetProjectName());
	CalculateDisplaySize();
}

void FPGameProjectSetting::SetWinClassName(std::wstring WinClassName)
{
	this->WinClassName = WinClassName;
}

void FPGameProjectSetting::SetWinName(std::wstring WinName)
{
	this->WinName = WinName;
}

void FPGameProjectSetting::SetWinWidth(int WinWidth)
{
	this->WinWidth = WinWidth;
}

void FPGameProjectSetting::SetWinHeight(int WinHeight)
{
	this->WinHeight = WinHeight;
}

void FPGameProjectSetting::SetDisplayWidth(int DisplayWidth)
{
	this->DisplayWidth = DisplayWidth;
}

void FPGameProjectSetting::SetDisplayHeight(int DisplayHeight)
{
	this->DisplayHeight = DisplayHeight;
}

std::wstring FPGameProjectSetting::GetWinClassName()
{
	return WinClassName;
}

std::wstring FPGameProjectSetting::GetWinName()
{
	return WinName;
}

int FPGameProjectSetting::GetWinWidth()
{
	return WinWidth;
}

int FPGameProjectSetting::GetWinHeight()
{
	return WinHeight;
}

int FPGameProjectSetting::GetDisplayWidth()
{
	return DisplayWidth;
}

int FPGameProjectSetting::GetDeisplayHeight()
{
	return DisplayHeight;
}

float FPGameProjectSetting::GetDisplayAspect()
{
	return DisplayAspect;
}
