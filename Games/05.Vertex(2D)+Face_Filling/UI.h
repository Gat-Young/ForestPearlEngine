#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"
#include "../../Engine/ForestPearlEngine/TextComponent.h"

class UI : public FPActor
{
	private:
		std::vector<TextComponent*> TextComponets;

	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
		void CalFPS(int x, int y);
		void ShowInfo();
		void SetUIContext(int index, int x, int y, unsigned long color, std::basic_string<TCHAR> text);
};