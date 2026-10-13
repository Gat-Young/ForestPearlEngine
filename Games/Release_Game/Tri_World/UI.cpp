#include "UI.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/ForestPearlEngine.h"
#include <iostream>
#include <Windows.h>

void UI::Initialize()
{
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
	Text9 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	SystemTitle = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	GPUDescriptionText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	FeatText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	ResText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	GridText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	AxisText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	CullText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	DepthText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	FillText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	NormalText = TextComponets.back();
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

	y += 14;

	_stprintf_s(text, _T("Grid:F2"));
	GridText->SetTextData(&bShow, x, y += 14, (GridOn == true) ? col : Col2, text);

	_stprintf_s(text, _T("Axis:F3"));
	AxisText->SetTextData(&bShow, x, y += 14, (AxisOn == true) ? col : Col2, text);

	_stprintf_s(text, _T("뒷면 제거:F4 (%s)"), ((isCull == true) ? _T("ON") : _T("OFF")));
	CullText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("깊이테스트:F5 (%s)"), ((ZEnable == true)? _T("ON") : _T("OFF")));
	DepthText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("채우기:SPACE (%s)"), ((isFill == true) ? _T("SOLID") : _T("WIRE")));
	FillText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("Normal:F6 (%s)"), ((bNormal == true) ? _T("ON") : _T("OFF")));
	NormalText->SetTextData(&bShow, x, y += 14, col, text);

}

void UI::AdapterInfo(int index, int x, int& y, FPVector4 col)
{
	
}

void UI::ShowInfo()
{	
	int x = 300, y = 5;

	FPVector4 col = { 1.0f, 1.0f, 1.0f, 1.0f };
	TCHAR text[1024];
	_stprintf_s(text, _T("■ %s"), _T("Tri World - 3"));
	Text1->SetTextData(&bShow, x, y, col, text);
	y += 15;
	Text2->SetTextData(&bShow, x, y += 15, col, _T("1. SpringArmComponent와 Possess"));
	Text3->SetTextData(&bShow, x, y += 15, col, _T("2. D-Pad LEFT/RIGHT로 Possess를 전환할 수 있습니다."));
	Text4->SetTextData(&bShow, x, y += 15, col, _T("3. L-Stick으로 이동, R-Stick으로 카메라를 회전 시킬 수 있습니다."));
	Text5->SetTextData(&bShow, x, y += 15, col, _T("4. Player와 풍차를 Possess로 전환해 이동해보세요!"));
	Text6->SetTextData(&bShow, x, y += 15, col, _T("5. SprigArmComponent는 Controller의 Rotation을 따라 회전합니다."));
	Text7->SetTextData(&bShow, x, y += 15, col, _T(""));
	Text8->SetTextData(&bShow, x, y += 15, col, _T("게임인재원 8기 프로그래밍학과 임백규"));

	Text9->SetTextData(&bShow, x, y += 15, { 1.0f, 0.0f, 0.0f, 1.0f }, _T("Have Fun~"));

	SystemInfo(1, 20, {1.0f, 1.0f, 0.0f, 1.0f});
}

void UI::SetUIContext( bool* actieve, int x, int y, FPVector4 color, std::basic_string<TCHAR> text)
{
	FPTextComponent* TextComponet = new FPTextComponent();
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

void UI::SetGridOn(struct FInputValue Value)
{
	GridOn = !(GridOn);
}
void UI::SetAxisOn(struct FInputValue Value)
{
	AxisOn = !(AxisOn);
}
void UI::SetCull(struct FInputValue Value)
{
	isCull = !(isCull);
}
void UI::SetFill(struct FInputValue Value)
{
	isFill = !(isFill);
}

void UI::SetNormalLine(FInputValue Value)
{
	bNormal = !(bNormal);
}
