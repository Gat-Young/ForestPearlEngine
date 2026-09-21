#include "FPSpringArmComponent.h"

void FPSpringArmComponent::CalculateTransformBranch()
{
	if (ParentComponent == nullptr)
	{
		WorldTransform = RelativeTransform;
	}
	else if(bUsePawnControlRotation)
	{
		//PlayerController를 찾아와서 해당 컨트롤러의 Rotation을 기준으로 회전을 적용
	}
	else
	{
		CalculateWorldTransform();
	}
}
