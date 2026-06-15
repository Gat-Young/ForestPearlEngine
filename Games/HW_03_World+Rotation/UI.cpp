#include "UI.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputMappingContext.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputAction.h"
#include "../../Engine/ForestPearlEngine/Systems/InputSystem.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include <iostream>
#include <Windows.h>

void UI::Initialize()
{
	int UI_count = 7;
	for (int i = 0; i < UI_count; ++i)
	{
		TextComponent* TextComponet = new TextComponent();
		TextComponets.push_back(TextComponet);
	}

	IMC = new FPInputMappingContext();
	FPInputSystem::GetInputSystem().AddActivatedIMC(IMC);

	IA = new FPInputAction();
	IA->BindMethod(this, EKeyState::Down, &UI::SetActiveViewHelp);


	FMappingInfo MappingInfoF1 = { IA , 0b00000000 };

	IMC->AddMappingKey(VK_F1, MappingInfoF1);
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
	static ULONGLONG oldtime = GetWorld()->GetGameTimer()->DeltaTimeMS();
	ULONGLONG		 nowtime = GetWorld()->GetGameTimer()->DeltaTimeMS();

	time += GetWorld()->GetGameTimer()->DeltaTimeMS();
	if (time >= 1000)
	{
		fps = (float)(frm * 1000) / (float)time;
		frm = 0;
		time = 0;
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
	_stprintf_s(text, _T("■ %s"), _T("HW_03_World+Rotation"));
	SetUIContext(1, bShow, x, y, col, text);
	y += 15;
	SetUIContext(2, bShow, x, y += 15, col, _T("1.정점 파이프라인(Vertex Pipeline) 의 이해"));
	SetUIContext(3, bShow, x, y += 15, col, _T("2.월드 변환 (World Transform): 스케일-회전-이동 변환 구현."));
	y += 15;
	SetUIContext(4, bShow, x, y += 15, col, _T("* 뷰-투영변환 없음 *"));
	y += 15;
	SetUIContext(5, bShow, x, y += 15, col, _T("3.B3Yena SW Renderer 의 결과와 비교해 봅시다."));
	SetUIContext(6, bShow, x, y += 15, col, _T("4.B3Yena SW Renderer 의 결과와 비교시 도움말을 끄십시요.(F1)"));
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
