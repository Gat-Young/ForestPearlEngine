#pragma once
#include "ForestPearlEngine/FPLevel.h"

class GameLevel : public FPLevel
{
	public:
		//상속해서 게임에 사용할 게임 오브젝트들을 만든다.
		virtual void Initialize() override;

		//상속 후 반드시 Super 할 것
		virtual void BeginPlay() override;
		virtual void Tick() override;
};