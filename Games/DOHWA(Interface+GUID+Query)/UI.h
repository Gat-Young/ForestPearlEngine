#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"

class UI : public FPActor
{
	public:
		virtual void BeginPlay() override;
		virtual void Tick() override;
		void CalFPS(int x, int y);
		void ShowInfo();
		void SetUIContext(int index, int x, int y, unsigned long color, std::basic_string<TCHAR> text);
};