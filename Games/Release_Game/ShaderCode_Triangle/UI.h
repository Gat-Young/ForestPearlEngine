#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/FPTextComponent.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;

class UI : public FPActor
{
	private:
		std::vector<FPTextComponent*> TextComponets;
		FPInputMappingContext*	IMC;
		FPInputAction* IA = nullptr;
		bool bShow = true;
		bool AlwaysOn = true;
		unsigned int time = 0;

		//UI 요소들
		FPTextComponent* FPSText;

		FPTextComponent* Text1;
		FPTextComponent* Text2;
		FPTextComponent* Text3;
		FPTextComponent* Text4;
		FPTextComponent* Text5;
		FPTextComponent* Text6;
		FPTextComponent* Text7;
		FPTextComponent* Text8;
		FPTextComponent* Text9;

		//장치 / GPU 및 시스템 정보 출력
		FPTextComponent* SystemTitle;
		FPTextComponent* GPUDescriptionText;
		FPTextComponent* FeatText;
		FPTextComponent* ResText;

		FPTextComponent* CullText;
		FPTextComponent* DepthText;
		FPTextComponent* FillText;

		bool ZEnable = true;
		bool isCull = false;
		bool isFill = true;

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
		void SetActiveDepthStencilBuffer(struct FInputValue Value);
		void SetCull(struct FInputValue Value);
		void SetFill(struct FInputValue Value);
};