#include "UI.h"
#include <iostream>
#include <Windows.h>

UI::UI()
{
	int UI_count = 14;
	for (int i = 0; i < 14; ++i)
	{
		UIContext* UIData = new UIContext();
		this->UI_data.push_back(UIData);
	}
}

void UI::BeginPlay()
{

}

void UI::Tick()
{
	ShowInfo();
	CalFPS(1, 1);

}

void UI::CalFPS(int x, int y)
{
	static UINT  frm = 0;
	static float fps = 0.0f;
	++frm;
	static ULONGLONG oldtime = GetTickCount64();
	ULONGLONG		 nowtime = GetTickCount64();

	UINT time = (UINT)(nowtime - oldtime);
	if (time >= 1000)
	{
		fps = (float)(frm * 1000) / (float)time;
		frm = 0;
		oldtime = nowtime;
	}

	TCHAR text[1024];
	_stprintf_s(text, _T("FPS=%.1f/%d"), fps, time);
	SetUIContext(0, x, y, RGB(255, 255, 255), text);
}

void UI::ShowInfo()
{
	static bool bShow = true;
	//if (IsKeyUp(VK_F1)) bShow ^= true;

	if (!bShow)
	{
		//DrawText(1,20, COLOR(1, 1, 0, 1), "[Help] F1"); 
		return;
	}


	// Today's Topic.
	{
		//int x = 350, y = 1;			
		int x = 800 / 2 - 100;
		int y = 50;
		COLORREF col = RGB(255, 255, 255);
		COLORREF col2 = RGB(255, 255, 0);
		COLORREF col3 = RGB(150, 150, 0);
		COLORREF col4 = RGB(150, 150, 150);
		COLORREF col5 = RGB(200, 200, 200);

		TCHAR text[64];
		_stprintf_s(text, _T("■ %s"), _T("DOHWA(Interface+GUID+Query)"));
		SetUIContext(1, x, y, col, text);
		SetUIContext(2, x, y += 14, col3, _T("1. 기본프레임워크 구축"));
		SetUIContext(3, x, y += 14, col2, _T("2. SW 렌더링 디바이스(Device) 생성"));
		SetUIContext(4, x, y += 14, col2, _T("  + SW COM / Interface 구축"));
		SetUIContext(5, x, y += 14, col2, _T("  + SW COM / Reference Count 구축"));
		SetUIContext(6, x, y += 14, col2, _T("  + SW COM / GUID 구축"));
		SetUIContext(7, x, y += 14, col3, _T("3. Idle 시간 렌더링 수행"));
		SetUIContext(8, x, y += 14, col3, _T("4. Swap-Chain 구현"));
	}


	int x = 800 / 2 - 100;
	int y = 300;
	static int cnt = 0;

	TCHAR text[64];
	_stprintf_s(text, _T("■ %s"), _T("Hello, Device!  cnt=%08d"), ++cnt);
	SetUIContext(9, x, y, RGB(255, 255, 0), text);

	y += 14;
	y += 14;
	SetUIContext(10, x, y += 14, RGB(200, 150, 200), _T("* B3Yena S/W Renderer 클래스 버전 *"));
	SetUIContext(11, x, y += 14, RGB(0, 200, 200), _T("* IYena S/W COM 인터페이스 적용 *"));
	SetUIContext(12, x, y += 14, RGB(0, 200, 200), _T("* IYena S/W COM 참조 카운트 적용 *"));
	SetUIContext(13, x, y += 14, RGB(0, 255, 255), _T("* IYena S/W COM GUID 적용 *"));
}

void UI::SetUIContext(int index, int x, int y, unsigned long color, std::basic_string<TCHAR> text)
{
	this->UI_data[index]->x = x;
	this->UI_data[index]->y = y;
	this->UI_data[index]->color = color;

	this->UI_data[index]->msg = text;
}
