#pragma once
#include "../ForestPearlEngine/Object/Actor.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////////
// 움직임, 인벤토리, 어트리뷰트 관리 및 기타 비물리적 개념과 같은 추상적인 동작 대부분에 유용
// 트랜스폼이 없으며, 액터 컴포넌트의 경우 월드 내 물리적 위치 또는 회전이 없다는 것을 의미
//
class FPActorComponent : public FPObject
{
	protected:
		FPActor* Owner;

	public:
		FPActorComponent(FPActor* Owner) : Owner(Owner) {};
		FPActor* GetOwner() { return Owner; };
};