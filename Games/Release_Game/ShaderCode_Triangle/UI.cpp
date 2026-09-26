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
	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetUITriangel", this, EKeyState::Down, &UI::SetActiveViewHelp);
	Controller->GetInputComponent().BindMethod("IA_SetDepthStencilBuffer", this, EKeyState::Down, &UI::SetActiveDepthStencilBuffer);
	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &UI::SetFill);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &UI::SetCull);
	Controller->GetInputComponent().BindMethod("IA_OnTripleCam", this, EKeyState::Down, &UI::SetTripleCam);

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
	CullText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	DepthText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	FillText = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	TripleCamText = TextComponets.back();

	//상수 버퍼 데이터 출력
	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	State1Text = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	State2Text = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	State3Text = TextComponets.back();

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

	_stprintf_s(text, _T("뒷면 제거:F4 (%s)"), ((isCull == true) ? _T("ON") : _T("OFF")));
	CullText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("깊이테스트:F5 (%s)"), ((ZEnable == true)? _T("ON") : _T("OFF")));
	DepthText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("채우기:SPACE (%s)"), ((isFill == true) ? _T("SOLID") : _T("WIRE")));
	FillText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("3분할화면:F6 (%s)"), ((isTripleCam == true) ? _T("ON") : _T("OFF")));
	TripleCamText->SetTextData(&bShow, x, y += 14, col, text);

}

void UI::ConstBufferInfo()
{
	FPVector4 Col = FPVector4{1.0f, 1.0f, 0.0f, 1.0f} *0.7f;

	ConstStateOn = (isTripleCam && bShow);
	TCHAR text[1024];

	int x = 250;
	State1Text->SetTextData(&ConstStateOn, x, 450, Col, _T("1. 원본 색상"));

	x = x + 600;
	State2Text->SetTextData(&ConstStateOn, x, 450, Col, _T("2. 색상 애니"));

	x = x + 600;
	State3Text->SetTextData(&ConstStateOn, x,450, Col, _T("3. 최종 혼합"));
}

void UI::AdapterInfo(int index, int x, int& y, FPVector4 col)
{
	
}

void UI::ShowInfo()
{	
	int x = 600, y = 5;

	FPVector4 col = { 1.0f, 1.0f, 1.0f, 1.0f };
	TCHAR text[1024];
	_stprintf_s(text, _T("■ %s"), _T("1 Model + 3 ViewPort + 3 ConstantBuffer"));
	Text1->SetTextData(&bShow, x, y, col, text);
	y += 15;
	Text2->SetTextData(&bShow, x, y += 15, col, _T("> 사용한 셰이더 코드 Demo.vsh, Demo.psh"));
	Text3->SetTextData(&bShow, x, y += 15, col, _T("> 애니메이션 색상을 만들고 넘기는 코드는 CB2Material.cpp에서 확인 가능합니다."));
	Text4->SetTextData(&bShow, x, y += 15, col, _T("> 사용한 버퍼들 : [VertexShader : MVP용, Material용, ViewPort용] | [PixelShader : Material용, ViewPort용]"));
	Text5->SetTextData(&bShow, x, y += 15, col, _T("> 총 5개가 할당 및 Set되어 있으나, 이번 예제에서는 VertexShader쪽 상수 버퍼의 값만 사용합니다."));
	Text6->SetTextData(&bShow, x, y += 15, col, _T("> Animation과 BlendOn 정보는 ViewPort의 상수 버퍼에 전달됩니다."));
	Text7->SetTextData(&bShow, x, y += 15, col, _T(""));
	Text8->SetTextData(&bShow, x, y += 15, col, _T("게임인재원 8기 프로그래밍학과 임백규"));

	Text9->SetTextData(&bShow, x, y += 15, { 1.0f, 0.0f, 0.0f, 1.0f }, _T("Have Fun~"));

	SystemInfo(1, 20, {1.0f, 1.0f, 0.0f, 1.0f});
	ConstBufferInfo();
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

void UI::SetCull(struct FInputValue Value)
{
	isCull = !(isCull);
}
void UI::SetFill(struct FInputValue Value)
{
	isFill = !(isFill);
}

void UI::SetTripleCam(FInputValue Value)
{
	isTripleCam = !(isTripleCam);
}
