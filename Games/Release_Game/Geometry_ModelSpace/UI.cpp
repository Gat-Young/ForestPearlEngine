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
	SystemText1 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	SystemText2 = TextComponets.back();

	SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
	SystemText3 = TextComponets.back();
	
	int AdpaterSize = ForestPearlEngine::GetGameEngine().GetAdapterSize();
	GPUState.resize(AdpaterSize);

	for (int i = 0; i < AdpaterSize; ++i)
	{
		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].GPUNumText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].AdapterText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].DescriptionText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].VendorIDText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].DeviceIdText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].SubsysIdText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].RevisionText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].TotalVideoMemText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].VideoMemText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].SystemMemText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].SharedSysMemText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].AdapterLuidText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].VRAMText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].VRAMBudgetText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].VRAMCurrUsageText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].VRAMAvailReservationText = TextComponets.back();

		SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
		GPUState[i].VRAMCurrReservedText = TextComponets.back();

		int MonitorDeviceSize = ForestPearlEngine::GetGameEngine().GetAdapterMonitorSize(i);
		GPUState[i].MonitorState.resize(MonitorDeviceSize);
		for (int j = 0; j < MonitorDeviceSize; ++j)
		{
			SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
			GPUState[i].MonitorState[j].MonitorNameText = TextComponets.back();

			SetUIContext(&bShow, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, _T(""));
			GPUState[i].MonitorState[j].MonitorRectText = TextComponets.back();
		}

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
	SystemText1->SetTextData(&bShow, x, y += 14, col, _T("[SYSTEM]"));

	TCHAR text[1024];

	_stprintf_s(text, _T("Feat: %s"), ForestPearlEngine::GetGameEngine().GetSrtFeatureLevel());
	SystemText2->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("Res: %dx%d"), ForestPearlEngine::GetGameEngine().GetWidth(), ForestPearlEngine::GetGameEngine().GetHeight());
	SystemText3->SetTextData(&bShow, x, y += 14, col, text);

	int AdapterSize = ForestPearlEngine::GetGameEngine().GetAdapterSize();

	int InfoY = y += 14;
	for (int i = 0; i < AdapterSize; ++i)
	{
		AdapterInfo(i, x, InfoY, col);
		InfoY += 14;
	}
	y = InfoY;

}

void UI::AdapterInfo(int index, int x, int& y, FPVector4 col)
{
	TCHAR text[1024];
	_stprintf_s(text, _T("[GPU #%d]"), index);
	GPUState[index].GPUNumText->SetTextData(&bShow, x, y += 14, { 0.0f, 1.0f, 0.0f, 1.0f }, text);
	//GPU 정보.
	_stprintf_s(text, _T("Adapter: %u"), index);
	GPUState[index].AdapterText->SetTextData(&bShow, x, y += 14, col, text);		//기본 장치만 처리함. 다중 GPU 구성시에는  열거처리가 필요.

	_stprintf_s(text, _T("Description: %s"), ForestPearlEngine::GetGameEngine().GetAdapterDescription(index));
	GPUState[index].DescriptionText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("VendorID: %u"), ForestPearlEngine::GetGameEngine().GetAdapterVendorID(index));
	GPUState[index].VendorIDText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("DeviceID: %u"), ForestPearlEngine::GetGameEngine().GetAdapterDeviceID(index));
	GPUState[index].DeviceIdText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("SubSysID: %u"), ForestPearlEngine::GetGameEngine().GetAdapterSubSysID(index));
	GPUState[index].SubsysIdText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("Revision: %u"), ForestPearlEngine::GetGameEngine().GetAdapterRevision(index));
	GPUState[index].RevisionText->SetTextData(&bShow, x, y += 14, col, text);

	SIZE_T DedicVRAM = ForestPearlEngine::GetGameEngine().GetAdapterVideoMem(index);
	SIZE_T DedicSYSM = ForestPearlEngine::GetGameEngine().GetAdapterSystemMem(index);
	SIZE_T SharedSYSM = ForestPearlEngine::GetGameEngine().GetAdapterSharedSysMem(index);
	SIZE_T TotalVRAM = DedicVRAM + DedicSYSM + SharedSYSM;
	_stprintf_s(text, _T("Total.VRAM: %lu GB (%lu MB)"),TotalVRAM/1000 ,TotalVRAM);
	GPUState[index].TotalVideoMemText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("Dedic.VRAM: %lu GB (%lu MB)"), DedicVRAM/1000, DedicVRAM);
	GPUState[index].VideoMemText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("Dedic.SYSM: %lu GB (% lu MB)"), DedicSYSM/1000, DedicSYSM);
	GPUState[index].SystemMemText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("Shared.SYSM: %lu GB (%lu MB)"), SharedSYSM/1000, SharedSYSM);
	GPUState[index].SharedSysMemText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("AdapterLuid: %u.%d"), ForestPearlEngine::GetGameEngine().GetAdapterLuidHighPart(index), ForestPearlEngine::GetGameEngine().GetAdapterLuidLowPart(index));
	GPUState[index].AdapterLuidText->SetTextData(&bShow, x, y += 14, col, text);

	_stprintf_s(text, _T("[VRAM]"));
	GPUState[index].VRAMText->SetTextData(&bShow, x, y += 14, {1.0f, 0.0f, 1.0f, 1.0f}, text);

	double VRAMBudget = ForestPearlEngine::GetGameEngine().GetVRAMBudget(index);
	_stprintf_s(text, _T("Budget: %.0f GB (%.0f MB)"), VRAMBudget/1000.0, VRAMBudget);
	GPUState[index].VRAMBudgetText->SetTextData(&bShow, x, y += 14, { 1.0f, 0.0f, 1.0f, 1.0f }, text);

	double VRAMCurrUsage = ForestPearlEngine::GetGameEngine().GetVRAMCurrUsage(index);
	_stprintf_s(text, _T("Curr.Usage: %.3f GB (%.0f MB) (%.2f%%)"), VRAMCurrUsage / 1000.0, VRAMCurrUsage, (VRAMCurrUsage / VRAMBudget) * 100.0f);
	GPUState[index].VRAMCurrUsageText->SetTextData(&bShow, x, y += 14, { 1.0f, 0.0f, 1.0f, 1.0f }, text);

	double VRAMAvailResevation = ForestPearlEngine::GetGameEngine().GetVRAMAvailableForReservation(index);
	_stprintf_s(text, _T("Avail.Reservation: %.0f GB (%.0f MB)"), VRAMAvailResevation / 1000.0, VRAMAvailResevation);
	GPUState[index].VRAMAvailReservationText->SetTextData(&bShow, x, y += 14, { 1.0f, 0.0f, 1.0f, 1.0f }, text);

	double VRAMCurrReservation = ForestPearlEngine::GetGameEngine().GetVRAMCurrReservation(index);
	_stprintf_s(text, _T("Curr.Reserved: %.0f GB (%.0f MB)"), VRAMCurrReservation / 1000.0, VRAMCurrReservation);
	GPUState[index].VRAMCurrReservedText->SetTextData(&bShow, x, y += 14, { 1.0f, 0.0f, 1.0f, 1.0f }, text);

	int MonitorDeviceSize = ForestPearlEngine::GetGameEngine().GetAdapterMonitorSize(index);
	GPUState[index].MonitorState.resize(MonitorDeviceSize);
	for (int j = 0; j < MonitorDeviceSize; ++j)
	{
		_stprintf_s(text, _T("Monitor #%d : %s"), j, ForestPearlEngine::GetGameEngine().GetMonitorName(index, j));
		GPUState[index].MonitorState[j].MonitorNameText->SetTextData(&bShow, x, y += 14, { 0.0f, 1.0f, 1.0f, 1.0f }, text);

		RECT MonitorRect = ForestPearlEngine::GetGameEngine().GetDesktopCoordinates(index, j);
		_stprintf_s(text, _T("MonitorRECT: {%d,%d,%d,%d}"), MonitorRect.left, MonitorRect.top, MonitorRect.right, MonitorRect.bottom);
		GPUState[index].MonitorState[j].MonitorRectText->SetTextData(&bShow, x, y += 14, { 0.0f, 1.0f, 1.0f, 1.0f }, text);
	}
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
