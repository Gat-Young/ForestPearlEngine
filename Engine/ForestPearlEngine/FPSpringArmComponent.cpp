#include "FPSpringArmComponent.h"
#include "./Object/FPPawn.h"
#include "FPAController.h"
#include <iostream>


FPSpringArmComponent::FPSpringArmComponent(FPActor* Owner) : FPSceneComponent(Owner)
{

}

void FPSpringArmComponent::CalculateTransformBranch()
{
	//SpringArmEndPoint가 고려된 분기를 만들 것
	if (ParentComponent == nullptr)
	{
		WorldTransform = RelativeTransform;
	}
	else if(bUsePawnControlRotation)
	{
		//PlayerController를 찾아와서 해당 컨트롤러의 Rotation을 기준으로 회전을 적용
		FPPawn* Pawn = static_cast<FPPawn*>(GetOwner());
		FPAController* Controller = Pawn->GetController();

		if (Controller)
		{
			FTransform ControllerTransform = Controller->GetActorTransform();

			FTransform ParentWorldTransform = ParentComponent->GetSocketTransform(ParentSocketName);

			CalculateWorldScale(ParentWorldTransform);
			CalculateWorldRotation(ControllerTransform);
			CalculateWorldLocation(ParentWorldTransform);
		}
	}
	else
	{
		CalculateWorldTransform();
	}
}

FTransform FPSpringArmComponent::GetSocketTransform(const std::string& SocketName) const
{
	//SpringArmEndPoint Socket까지 고려된 위치를 반환 할 것
	
	//SpringArmEndPoint의 Transform
	FTransform SpringArmEndPoint;

	//+z축을 앞 방향 벡터로 할 것 (DX11 왼손 좌표계를 기준으로)
	FPVector3 Forward = { 0.0f, 0.0f, 1.0f };
	Forward = Rotate(WorldTransform.QuaternionRotation , Forward);
	
	//Scale 계산 (부모 월드 스케일 * 로컬 스케일)
	SpringArmEndPoint.Scale = FPVector3{ 1.0f, 1.0f, 1.0f };

	//Rotation 계산 (부모 사원수 회전 * 로컬 사원수 회전)
	SpringArmEndPoint.QuaternionRotation = WorldTransform.QuaternionRotation.Normalize();
	SpringArmEndPoint.Rotation = SpringArmEndPoint.QuaternionRotation.ToEuler();

	//Location 계산 (부모 위치 + Rotate(부모 회전 사원수, (자식 위치 * 부모 크기)) 
	SpringArmEndPoint.Location = WorldTransform.Location +
		Rotate(WorldTransform.QuaternionRotation, (RelativeTransform.Location * WorldTransform.Scale));

	//-x축 방향으로 TargetArmLength 거리 만큼 뒤에 위치
	FPVector3 BackForwardSocketPosition = -Forward * TargetArmLength;
	SpringArmEndPoint.Location = (SpringArmEndPoint.Location + BackForwardSocketPosition);

	//std::cout << "[Spring Arm EndPoint] : " << SpringArmEndPoint.Location.x << " , " << SpringArmEndPoint.Location.y << " , " << SpringArmEndPoint.Location.z << "\n";
	//std::cout << "[-Forward * TargetArmLength] : " << (-Forward * TargetArmLength).x << " , " << (-Forward * TargetArmLength).y << " , " << (-Forward * TargetArmLength).z << "\n";


	return SpringArmEndPoint;
}
