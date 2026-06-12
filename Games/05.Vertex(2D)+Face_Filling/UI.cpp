#include "UI.h"
//#include "../../Engine/ForestPearlEngine/Object/Components/InputMappingContext.h"
//#include "../../Engine/ForestPearlEngine/Object/Components/InputAction.h"
//#include "../../Engine/ForestPearlEngine/Systems/InputSystem.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include <iostream>
#include <Windows.h>

void UI::Initialize()
{
	InputComponent = new FPInputComponent();

	int UI_count = 6;
	for (int i = 0; i < UI_count; ++i)
	{
		TextComponent* TextComponet = new TextComponent();
		TextComponets.push_back(TextComponet);
	}

	FMappingInfo MappingInfoF1 = { ESwizzle::YZX , ENegative::Positive };
	InputComponent->AddMappingKey(VK_F1, MappingInfoF1);
	InputComponent->BindMethod(this, EKeyState::Down, &UI::SetActiveViewHelp);

	//IMC = new FPInputMappingContext();
	//FPInputSystem::GetInputSystem().AddActivatedIMC(IMC);

	//IA = new FPInputAction();
	//IA->BindMethod(this, EKeyState::Down, &UI::SetActiveViewHelp);


	//FMappingInfo MappingInfoF1 = { IA , 0b00000000 };

	//IMC->AddMappingKey(VK_F1, MappingInfoF1);
}


void UI::BeginPlay()
{
}


void UI::Tick()
{
	InputComponent->ProcessInputTick();
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
	SetUIContext(0,true, x, y, RGB(255, 255, 255), text);
}

void UI::ShowInfo()
{	
	int x = 300, y = 50;

	COLORREF col = RGB(255, 255, 255);
	TCHAR text[1024];
	_stprintf_s(text, _T("■ %s"), _T("DX(05.Vertex(2D)+Face_Filling)"));
	SetUIContext(1, bShow, x, y, col, text);
	y += 15;
	SetUIContext(2, bShow, x, y += 15, col, _T("1.정점, 정점버퍼를 구성합니다."));
	SetUIContext(3, bShow, x, y += 15, col, _T("2.정점색 보간 결과를 확인합니다."));
	SetUIContext(4, bShow, x, y += 15, col, _T("3.B3Yena SW Renderer 의 결과와 비교해 봅시다."));
	SetUIContext(5, bShow, x, y += 15, col, _T("4.B3Yena SW Renderer 의 결과와 비교시 도움말을 끄십시요.(F1)"));
}

void UI::SetUIContext(int index, bool actieve, int x, int y, unsigned long color, std::basic_string<TCHAR> text)
{
	this->TextComponets[index]->SetTextData(actieve, x, y, color, text);
}

void UI::SetActiveViewHelp(FPVector2 value)
{
	std::cout << "F1 : ";
	bShow = !(bShow);
	std::cout << bShow << "\n";
}
