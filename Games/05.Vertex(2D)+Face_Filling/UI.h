#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"
#include "../../Engine/ForestPearlEngine/TextComponent.h"
#include "../../Engine/ForestPearlEngine/Systems/KeyStateEnum.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"

//class FPInputMappingContext;
//class FPInputAction;
class FPInputComponent;

class UI : public FPActor
{
	private:
		std::vector<TextComponent*> TextComponets;

		FPInputComponent* InputComponent;

		//FPInputMappingContext*	IMC;
		//FPInputAction* IA = nullptr;
		bool bShow = true;

	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
		void CalFPS(int x, int y);
		void ShowInfo();
		void SetUIContext(int index, bool actieve, int x, int y, unsigned long color, std::basic_string<TCHAR> text);
		void SetActiveViewHelp(FPVector2 value);
		FPInputComponent& GetInputComponent() { return *InputComponent; }
};