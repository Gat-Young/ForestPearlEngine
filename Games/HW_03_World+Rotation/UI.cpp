#include "UI.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include <iostream>
#include <Windows.h>

void UI::Initialize()
{

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetUITriangel", this, EKeyState::Pressed, &UI::SetActiveViewHelp);
	SetUIContext(true, 1, 1, RGB(255, 255, 255), _T(""));
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
	this->TextComponets[0]->SetTextData(true, x, y, RGB(255, 255, 255), text);
}

void UI::ShowInfo()
{	
	int x = 300, y = 50;

	COLORREF col = RGB(255, 255, 255);
	TCHAR text[1024];
	_stprintf_s(text, _T("■ %s"), _T("HW_03_World+Rotation"));
	SetUIContext( bShow, x, y, col, text);
	y += 15;
	SetUIContext( bShow, x, y += 15, col, _T("1.정점 파이프라인(Vertex Pipeline) 의 이해"));
	SetUIContext( bShow, x, y += 15, col, _T("2.월드 변환 (World Transform): 스케일-회전-이동 변환 구현."));
	y += 15;
	SetUIContext( bShow, x, y += 15, col, _T("* 뷰-투영변환 없음 *"));
	y += 15;
	SetUIContext( bShow, x, y += 15, RGB(255, 255, 0), _T("* 정점 파이프라인 (Vertex Pipeline) 구현."));
	SetUIContext( bShow, x, y += 15, RGB(255, 255, 0), _T("* 기하 파이프라인 (Geometry Pipeline) 구현."));
	SetUIContext( bShow, x, y += 15, RGB(255, 255, 0), _T("* 픽셀 파이프라인 (Pixel Pipeline) 구현."));

	SetUIContext( bShow, x, y += 15, RGB(255, 0, 0), _T("행렬 클래스 및 함수 제작"));
}

void UI::SetUIContext( bool actieve, int x, int y, unsigned long color, std::basic_string<TCHAR> text)
{
	TextComponent* TextComponet = new TextComponent();
	TextComponet->SetTextData(actieve, x, y, color, text);
	TextComponets.push_back(TextComponet);
}

void UI::SetActiveViewHelp(FInputValue Value)
{
	std::cout << "F1 : ";
	bShow = !(bShow);
	std::cout << bShow << "\n";
}
