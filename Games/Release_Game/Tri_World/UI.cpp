#include "UI.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/GameTimer.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/ForestPearlEngine.h"
#include <iostream>
#include <Windows.h>

void UI::Initialize()
{
	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetUITriangel", this, EKeyState::Down, &UI::SetActiveViewHelp);
	Controller->GetInputComponent().BindMethod("IA_SetDepthStencilBuffer", this, EKeyState::Down, &UI::SetActiveDepthStencilBuffer);

	SetUIContext(&AlwaysOn, 1, 1, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	FPSText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	Text1 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	Text2 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	Text3 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	Text4 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	Text5 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	Text6 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	Text7 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	Text8 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	SystemTitle = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	GPUDescriptionText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	FeatText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	ResText = TextComponets.back();
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
	FPSText->SetTextData(&AlwaysOn, x, y, { 1.0f, 1.0f, 1.0f, 1.0f }, text);
}

void UI::SystemInfo(int x, int y, FPVector4 col)
{
	FPVector4 Col2 = col * 0.7f;
	SystemTitle->SetTextData(&bShow, x, y += 14, col, _T("[SYSTEM]"));

	TCHAR text[1024];

	_stprintf_s(text, _T("GPU: %s"), ForestPearlEngine::GetGameEngine().GetAdapterDescription(0));
	GPUDescriptionText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("Feat: %s"), ForestPearlEngine::GetGameEngine().GetSrtFeatureLevel());
	FeatText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("Res: %dx%d"), ForestPearlEngine::GetGameEngine().GetWidth(), ForestPearlEngine::GetGameEngine().GetHeight());
	ResText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("깊이테스트:F5 (%s)"), ((ZEnable == true)? _T("ON") : _T("OFF")));
	ResText->SetTextData(&bShow, x, y += 14, col, text);

}

void UI::AdapterInfo(int index, int x, int& y, FPVector4 col)
{
	
}

void UI::ShowInfo()
{	
	int x = 300, y = 50;

	FPVector4 col = { 1.0f, 1.0f, 1.0f, 1.0f };
	TCHAR text[1024];
	_stprintf_s(text, _T("■ %s"), _T("GPU Info"));
	Text1->SetTextData(&bShow, x, y, col, text);
	y += 15;
	Text2->SetTextData(&bShow, x, y += 15, col, _T("1.기본프레임워크 구축"));
	Text3->SetTextData(&bShow, x, y += 15, col, _T("2.HW 렌더링 디바이스(DX11 Device)를 생성"));
	Text4->SetTextData(&bShow, x, y += 15, col, _T("3.Idle 시간 렌더링"));
	Text5->SetTextData(&bShow, x, y += 15, col, _T("4.스왑체인 Swap(Flipping) chain의 이해"));
	Text6->SetTextData(&bShow, x, y += 15, col, _T("5.전체화면 또는 창모드 전환 (Alt-Enter)"));
	Text7->SetTextData(&bShow, x, y += 15, col, _T("6.수직동기화(VSync): 티어링(Tearing), 셔터링(Shuttering) 방지"));

	Text8->SetTextData(&bShow, x, y += 15, { 1.0f, 0.0f, 0.0f, 1.0f }, _T("7.장치 및 GPU 정보 획득 (DXGI 1.0)"));

	SystemInfo(1, 20, {1.0f, 1.0f, 0.0f, 1.0f});
}

void UI::SetUIContext( bool* actieve, int x, int y, FPVector4 color, std::basic_string<TCHAR> text)
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

void UI::SetActiveDepthStencilBuffer(FInputValue Value)
{
	std::cout << "F5 : ";
	ZEnable = !(ZEnable);
	ForestPearlEngine::GetGameEngine().SetZEnable(ZEnable);
	std::cout << ZEnable << "\n";
}
