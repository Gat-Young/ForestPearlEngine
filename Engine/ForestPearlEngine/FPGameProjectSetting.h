#pragma once
#include <string>
#include <unordered_map>
#include <Windows.h>
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"

class FPGameProjectSetting : public FPGameInstanceSubSystem
{
	private:
		std::wstring WinClassName;
		std::wstring WinName;
		int WinWidth;
		int WinHeight;
		int DisplayWidth;
		int DisplayHeight;
		float DisplayAspect;
		HWND hwnd;

		void CalculateDisplaySize();

	public:
		FPGameProjectSetting();
		~FPGameProjectSetting() = default;

		void SetHWND(HWND hwnd);

		void SetWinClassName(std::wstring WinClassName);
		void SetWinName(std::wstring WinName);
		void SetWinWidth(int WinWidth);
		void SetWinHeight(int WinHeight);
		void SetDisplayWidth(int DisplayWidth);
		void SetDisplayHeight(int DisplayHeight);

		std::wstring GetWinClassName();
		std::wstring GetWinName();
		int GetWinWidth();
		int GetWinHeight();
		int GetDisplayWidth();
		int GetDeisplayHeight();
		float GetDisplayAspect();
};