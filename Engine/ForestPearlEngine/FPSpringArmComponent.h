#pragma once
#include "FPSceneComponent.h"

class FPSpringArmComponent : public FPSceneComponent
{
	protected:
		void CalculateTransformBranch() override;
		FTransform GetSocketTransform(const std::string& SocketName) const override;

		//추후에 SpringArmEndPoint가 벽 등에 충돌해 위치를 옮기는 멤버 함수를 추가

	public:

		//true일 경우 PlayerController에 회전을 따라감
		bool bUsePawnControlRotation = false;

		///////////////////////////////////
		//
		//	SpringArmEndPoint Socket
		//
		//로컬 X축에 반대 방향으로 TargetArmLength 만큼의 Location이 자식 컴포넌트의 위치가 됨
		float TargetArmLength = 100.0f;
		
		//SpringArmEndPoint의 Transform
		FTransform SpringArmEndPoint;


		///////////////////////////////////
};