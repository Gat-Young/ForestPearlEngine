#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/TextComponent.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;

class UI : public FPActor
{
	private:
		std::vector<TextComponent*> TextComponets;
		FPInputMappingContext*	IMC;
		FPInputAction* IA = nullptr;
		bool bShow = true;
		bool AlwaysOn = true;
		unsigned int time = 0;

		//UI 요소들
		TextComponent* FPSText;

		TextComponent* Text1;
		TextComponent* Text2;
		TextComponent* Text3;
		TextComponent* Text4;
		TextComponent* Text5;
		TextComponent* Text6;
		TextComponent* Text7;
		TextComponent* Text8;

		//장치 / GPU 및 시스템 정보 출력
		TextComponent* SystemText1;
		TextComponent* SystemText2;
		TextComponent* SystemText3;

		struct MonitorStateText
		{
			TextComponent* MonitorNameText;
			TextComponent* MonitorRectText;
		};

		struct GPUStateText
		{
			TextComponent* GPUNumText;
			TextComponent* AdapterText;
			TextComponent* DescriptionText;
			TextComponent* VendorIDText;
			TextComponent* DeviceIdText;
			TextComponent* SubsysIdText;
			TextComponent* RevisionText;
			TextComponent* TotalVideoMemText;
			TextComponent* VideoMemText;
			TextComponent* SystemMemText;
			TextComponent* SharedSysMemText;
			TextComponent* AdapterLuidText;

			TextComponent* VRAMText;
			TextComponent* VRAMBudgetText;
			TextComponent* VRAMCurrUsageText;
			TextComponent* VRAMAvailReservationText;
			TextComponent* VRAMCurrReservedText;

			std::vector<MonitorStateText> MonitorState;
		};
		std::vector<GPUStateText> GPUState;


	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void CalFPS(int x, int y);
		void SystemInfo(int x, int y, FPVector4 col);
		void AdapterInfo(int index, int x, int& y, FPVector4 col);
		void ShowInfo();
		void SetUIContext(bool* actieve, int x, int y, FPVector4 color, std::basic_string<TCHAR> text);
		void SetActiveViewHelp(struct FInputValue Value);
};