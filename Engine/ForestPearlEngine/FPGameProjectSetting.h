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
		int WinWidth = 960;		//기본 윈도우 사이즈
		int WinHeight = 600;	//기본 윈도우 사이즈
		int DisplayWidth;
		int DisplayHeight;
		float DisplayAspect = 960.0f / 600.0f;		//기본 가로:세로 비율

		HWND hwnd;


	public:
		FPGameProjectSetting() = default;
		~FPGameProjectSetting() = default;

		void SetHWND(HWND hwnd);
		
		void SetWinClassName(std::wstring WinClassName);
		void SetWinName(std::wstring WinName);
		void SetWinWidth(int WinWidth);
		void SetWinHeight(int WinHeight);
		void SetDisplayWidth(int DisplayWidth);
		void SetDisplayHeight(int DisplayHeight);
		void ProjectSetting();

		std::wstring GetWinClassName();
		std::wstring GetWinName();
		int GetWinWidth();
		int GetWinHeight();
		int GetDisplayWidth();
		int GetDeisplayHeight();
		float GetDisplayAspect();

		void CalculateDisplaySize();
};