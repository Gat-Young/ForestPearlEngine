#include "FPGameProjectSetting.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include "Utility/FPPathManager.h"
#include "FPAssetManager.h"

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

void FPGameProjectSetting::SetFullScreen(bool FullScreen)
{
	this->FullScreen = FullScreen;
}

void FPGameProjectSetting::SetVSync(bool VSync)
{
	this->VSync = VSync;
}

void FPGameProjectSetting::SetHz(int Hz)
{
	this->Hz = Hz;
}

void FPGameProjectSetting::ProjectSetting()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	FPGameSettingData* GameSettingData = AssetManager->GetGameSettingData();

	WinClassName = FPPathManager::Get().StringToWString(GameSettingData->GameName);
	WinName = FPPathManager::Get().StringToWString(GameSettingData->GameName);
	WinWidth = GameSettingData->Width;
	WinHeight = GameSettingData->Height;
	DisplayAspect = (float)WinWidth / (float)WinHeight;
	MSAA = GameSettingData->MSAA;
	AF = GameSettingData->AnisotropicFiltering;
	FullScreen = GameSettingData->FullScreen;

	VSync = GameSettingData->VSync;
	Hz = GameSettingData->Hz;
}

void FPGameProjectSetting::SyncGameSettingData()
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	FPGameSettingData* GameSettingData = AssetManager->GetGameSettingData();

	GameSettingData->Width = DisplayWidth;
	GameSettingData->Height = DisplayHeight;
	GameSettingData->MSAA = MSAA;
	GameSettingData->AnisotropicFiltering = AF;
	GameSettingData->FullScreen = FullScreen;
	GameSettingData->VSync = VSync;
	GameSettingData->Hz = Hz;
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

void FPGameProjectSetting::SetMSAA(int MSAA)
{
	if ((MSAA % 2 != 0) || (MSAA > 8))
	{
		this->MSAA = 1;
		return;
	} 
	this->MSAA = MSAA;
}

void FPGameProjectSetting::SetAF(int AF)
{
	if ((AF % 2 != 0) || (AF > 16))
	{
		this->AF = 1;
		return;
	}
	this->AF = AF;
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

int FPGameProjectSetting::GetMSAA()
{
	return MSAA;
}

int FPGameProjectSetting::GetAF()
{
	return AF;
}

bool FPGameProjectSetting::GetFullScreen()
{
	return FullScreen;
}

bool FPGameProjectSetting::GetVSync()
{
	return VSync;
}

int FPGameProjectSetting::GetHz()
{
	return Hz;
}
