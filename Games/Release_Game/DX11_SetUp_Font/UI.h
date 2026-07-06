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

	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
		void CalFPS(int x, int y);
		void ShowInfo();
		void SetUIContext(bool* actieve, int x, int y, FPVector4 color, std::basic_string<TCHAR> text);
		void SetActiveViewHelp(struct FInputValue Value);
};