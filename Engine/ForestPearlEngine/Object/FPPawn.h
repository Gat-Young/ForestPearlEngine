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
		FPAController* GetController() const;

		virtual void PossessedBy(FPAController* NewController);
		virtual void UnPossesed();
};