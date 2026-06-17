#include "UI.h"
//#include "../../Engine/ForestPearlEngine/Object/Components/InputMappingContext.h"
//#include "../../Engine/ForestPearlEngine/Object/Components/InputAction.h"
//#include "../../Engine/ForestPearlEngine/Systems/InputSystem.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/FPAController.h"
#include "../../Engine/ForestPearlEngine/Object/Components/InputComponent.h"
#include "../../Engine/ForestPearlEngine/InputValue.h"
#include <iostream>
#include <Windows.h>

void UI::Initialize()
{
	int UI_count = 6;
	for (int i = 0; i < UI_count; ++i)
	{
		TextComponent* TextComponet = new TextComponent();
		TextComponets.push_back(TextComponet);
	}

	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	FModifyInfo ModifyInfoF1 = { ESwizzle::YZX , ENegative::Positive };
	Controller->GetInputComponent().AddMappingKey("IA_InfoOff", VK_F1, ModifyInfoF1);

	Controller->GetInputComponent().BindMethod("IA_InfoOff", this, EKeyState::Pressed, &UI::SetActiveViewHelp);

	//InputComponent->BindMethod(this, EKeyState::Down, &UI::SetActiveViewHelp);

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
	int x = 300, y = 20;

	COLORREF col = RGB(255, 255, 255);
	TCHAR text[1024];
	_stprintf_s(text, _T("■ %s"), _T("ForestPearlEngine"));
	SetUIContext(1, bShow, x, y, col, text);
	y += 15;
	SetUIContext(2, bShow, x, y += 15, col, _T("1. 가운데 부모(Sun)를 중심으로 회전합니다."));
	SetUIContext(3, bShow, x, y += 15, col, _T("2. 부모를 중심으로 회전하는 자식(Planet)을 부모로 회전합니다."));
	SetUIContext(4, bShow, x, y += 15, col, _T("3. 화면을 왼쪽 클릭하면 해당 위치에서 Planet이 스폰됩니다."));
	SetUIContext(4, bShow, x, y += 15, col, _T("4. 화면을 오른쪽 클릭하면 Planet을 선택했는지 확인합니다."));
}

void UI::SetUIContext(int index, bool actieve, int x, int y, unsigned long color, std::basic_string<TCHAR> text)
{
	this->TextComponets[index]->SetTextData(&actieve, x, y, color, text);
}

void UI::SetActiveViewHelp(FInputValue value)
{
	std::cout << "F1 : ";
	bShow = !(bShow);
	std::cout << bShow << "\n";
}
