#include "UI.h"
#include <iostream>
#include <Windows.h>

void UI::Initialize()
{
	int UI_count = 5;
	for (int i = 0; i < UI_count; ++i)
	{
		TextComponent* TextComponet = new TextComponent();
		TextComponets.push_back(TextComponet);
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

	TCHAR text[64];
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

	
		int x = 300, y = 50;

		COLORREF col = RGB(255, 255, 255);
		TCHAR text[1024];
		_stprintf_s(text, _T("■ %s"), _T("DX(05.Vertex(2D)+Face_Filling)"));
		SetUIContext(1, x, y, col, text);
		y += 15;
		SetUIContext(2,x, y += 15, col, _T("1.정점, 정점버퍼를 구성합니다."));
		SetUIContext(3,x, y += 15, col, _T("2.정점색 보간 결과를 확인합니다."));
		SetUIContext(4,x, y += 15, col, _T("3.B3Yena SW Renderer 의 결과와 비교해 봅시다."));
}

void UI::SetUIContext(int index, int x, int y, unsigned long color, std::basic_string<TCHAR> text)
{
	this->TextComponets[index]->SetTextData(x, y, color, text);
}
