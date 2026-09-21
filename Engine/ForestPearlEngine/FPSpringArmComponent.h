#pragma once
#include "FPSceneComponent.h"

class FPSpringArmComponent : public FPSceneComponent
{
	protected:
		void CalculateTransformBranch() override;

	public:
		
		//로컬 X축에 반대 방향으로 TargetArmLength 만큼의 Location이 자식 컴포넌트의 위치가 됨
		float TargetArmLength = 100.0f;
		/*
		*	방법. SpringArmComponent에 보이지 않는 offset 자식 컴포넌트를 붙이고 자동으로 offset 자식 컴포넌트 밑에 붙도록 만든다. (Socket)
		*	- SpringArmComponet의 자식 컴포넌트는 Socket에 붙이도록 한다.
		*/


		//true일 경우 PlayerController에 회전을 따라감
		bool bUsePawnControlRotation = false;
};