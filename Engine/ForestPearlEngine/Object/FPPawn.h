#pragma once
#include "Actor.h"

//////////////////////////////
//
//	PlayerController가 Posses 할 수 있는 Class
//

class FPAController;

class FPPawn : public FPActor
{
	protected:
		FPAController* Controller;

	public:
		//Controller의 Rotation을 사용하는지에 대한 속성 (추후 구현)
		bool bUseControllerRotationYaw = false;
		bool bUseControllerRotationPitch = false;
		bool bUseControllerRotationRoll = false;

	public:
		FPAController* GetController() const;

		virtual void PossessedBy(FPAController* NewController);
		virtual void UnPossesed();

		//ControllerRotation 증가용 함수
		void AddControllerYawInput(float Value);
		void AddControllerPitchInput(float Value);
		void AddControllerRollInput(float Value);

};