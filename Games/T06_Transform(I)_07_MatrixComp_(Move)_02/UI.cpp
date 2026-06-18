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
	SetUIContext(&AlwaysOn, 1, 1, RGB(255, 255, 255), _T(""));
}


void UI::BeginPlay()
{
	ShowInfo();
}


void UI::Tick()
{
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
	this->TextComponets[0]->SetTextData(&AlwaysOn, x, y, RGB(255, 255, 255), text);
}

void UI::ShowInfo()
{	
	int x = 200, y = 5;

	COLORREF col = RGB(255, 255, 255);
	TCHAR text[1024];
	_stprintf_s(text, _T("■ %s"), _T("T06 Transform (I) 07 Matrix Composition (행렬결합+이동)(DX.Math)(GDI)(Ready)"));
	SetUIContext(&bShow, x, y, col, text);
	y += 15;
	SetUIContext(&bShow, x, y += 15, col, _T("1. 렌더링 파이프라인(Rendering Pipeline) 의 이해."));
	SetUIContext(&bShow, x, y += 15, col, _T("2. 월드 변환 : 이동/회전/스케일 행렬 결합 연습"));
	SetUIContext(&bShow, x, y += 15, col, _T("3. 뷰 변환 : 다양한 카메라 설정 연습"));
	SetUIContext(&bShow, x, y += 15, col, _T("4. 그리드 및 방향축 추가"));

	y += 15;
	SetUIContext(&bShow, x, y += 15, RGB(255, 255, 0), _T("* 정점 파이프라인 (Vertex Pipeline) 구현."));
	SetUIContext(&bShow, x, y += 15, RGB(255, 255, 0), _T("* 기하 파이프라인 (Geometry Pipeline) 구현."));
	SetUIContext(&bShow, x, y += 15, RGB(255, 255, 0), _T("* 픽셀 파이프라인 (Pixel Pipeline) 구현."));

	SetUIContext(&bShow, x, y += 15, RGB(255, 0, 0), _T("* 그리드 및 방향축 : class + DX행렬 + GDI그리기"));
	SetUIContext(&bShow, x, y += 15, RGB(255, 0, 0), _T("* 키보드로 '주인공' 움직이기"));
}

void UI::SetUIContext( bool* actieve, int x, int y, unsigned long color, std::basic_string<TCHAR> text)
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
