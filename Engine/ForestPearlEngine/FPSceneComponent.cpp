#include "FPSceneComponent.h"
#include <algorithm>
#include <iostream>

FPSceneComponent::FPSceneComponent(FPActor* Owner) : FPActorComponent(Owner)
{
}

//Transform 직접 설정
void FPSceneComponent::SetRelativeLocation(FPVector3 Location)
{
	RelativeTransform.Location = Location;
	CalculateWorldTransform();
}

void FPSceneComponent::SetRelativeRotation(FPVector3 Rotation)
{
	RelativeTransform.Rotation = Rotation;
	RelativeTransform.QuaternionRotation = FromEuler(Rotate({ 0.0f, 0.0f, 0.0f, 1.0f }, Rotation));
	CalculateWorldTransform();
}

void FPSceneComponent::SetRelativeScale3D(FPVector3 Scale)
{
	RelativeTransform.Scale = Scale;
	CalculateWorldTransform();
}

void FPSceneComponent::SetWorldLocation(FPVector3 Location)
{
	WorldTransform.Location = Location;
	CalculateLocalTransform();
}

void FPSceneComponent::SetWorldRotation(FPVector3 Rotation)
{
	WorldTransform.Rotation = Rotation;
	WorldTransform.QuaternionRotation = FromEuler(Rotate({ 0.0f, 0.0f, 0.0f, 1.0f }, Rotation));
	CalculateLocalTransform();
}

void FPSceneComponent::SetWorldScale3D(FPVector3 Scale)
{
	WorldTransform.Scale = Scale;
	CalculateLocalTransform();
}

//Transform 변화 값 추가

void FPSceneComponent::AddRelativeLocation(FPVector3 Offset)
{
	RelativeTransform.Location = RelativeTransform.Location + Offset;

	//월드를 재계산
	CalculateWorldTransform();
}

void FPSceneComponent::AddRelativeRotation(FPVector3 Rotation)
{
	//부모 축 기준으로 Rotation 회전
	FPQuaternion DeltaQuat = FromEuler(Rotation);
	RelativeTransform.QuaternionRotation = (DeltaQuat * ParentComponent->WorldTransform.QuaternionRotation).Normalize();
	RelativeTransform.Rotation = RelativeTransform.QuaternionRotation.ToEuler();

	//월드를 재계산
	CalculateWorldTransform();
}

void FPSceneComponent::AddLocalOffset(FPVector3 Offset)
{
	FPVector3 RotatedOffset =
		Rotate(
			RelativeTransform.QuaternionRotation,
			Offset
		);

	RelativeTransform.Location = RelativeTransform.Location + RotatedOffset;

	//월드를 재계산
	CalculateWorldTransform();
}

void FPSceneComponent::AddLocalRotation(FPVector3 Rotation)
{
	FPQuaternion DeltaQuat = FromEuler(Rotation);
	FPQuaternion NewQuaternionRotation = RelativeTransform.QuaternionRotation * DeltaQuat;
	RelativeTransform.QuaternionRotation = NewQuaternionRotation.Normalize();
	RelativeTransform.Rotation = RelativeTransform.QuaternionRotation.ToEuler();

	//월드를 재계산
	CalculateWorldTransform();
}

//월드 기준 변화량 추가
void FPSceneComponent::AddWorldOffset(FPVector3 Offset)
{
	FPVector3 NewLocation = WorldTransform.Location + Offset;
	WorldTransform.Location = NewLocation;

	//부모 기준의 로컬 Location을 재계산
	CalculateLocalTransform();
}

void FPSceneComponent::AddWorldRotation(FPVector3 Rotation)
{
	FPQuaternion DeltaQuat = FromEuler(Rotation);
	FPQuaternion NewQuaternionRotation = WorldTransform.QuaternionRotation * DeltaQuat;
	WorldTransform.QuaternionRotation = NewQuaternionRotation.Normalize();
	WorldTransform.Rotation = WorldTransform.QuaternionRotation.ToEuler();

	//부모 기준의 로컬 Rotation을 재계산
	CalculateLocalTransform();
}

//Transform 정보 반환
FTransform FPSceneComponent::GetComponentTransform()
{
	return WorldTransform;
}

FPVector3 FPSceneComponent::GetComponentLocation()
{
	return WorldTransform.Location;
}

FPVector3 FPSceneComponent::GetComponentRotation()
{
	return WorldTransform.Rotation;
}

FPQuaternion FPSceneComponent::GetComponentQuat()
{
	return WorldTransform.QuaternionRotation;
}

FPVector3 FPSceneComponent::GetComponentScale()
{
	return WorldTransform.Scale;
}

FTransform FPSceneComponent::GetRelativeTransform()
{
	return RelativeTransform;
}

FPVector3 FPSceneComponent::GetRelativeLocation()
{
	return RelativeTransform.Location;
}

FPVector3 FPSceneComponent::GetRelativeRotation()
{
	return RelativeTransform.Rotation;
}

FPVector3 FPSceneComponent::GetRelativeScale3D()
{
	return RelativeTransform.Scale;
}

//임시로 사용
void FPSceneComponent::Tick()
{
	if (ParentComponent == nullptr)
	{
		WorldTransform = RelativeTransform;
	}
	else
	{
		CalculateWorldTransform();
	}

	for (FPSceneComponent* child : ChildComponent)
	{
		child->Tick();
	}
}

//컴포넌트 등록 및 해제
void FPSceneComponent::SetupAttachment(FPSceneComponent* Parent)
{
	DetachFromComponent();
	ParentComponent = Parent;
	ParentComponent->AttachChildComponent(this);
	CalculateLocalTransform();
}

void FPSceneComponent::DetachFromComponent()
{
	if (ParentComponent == nullptr) return;
	ParentComponent->DetachChildComponent(this);
	//현재 World 값이 Local 값이 됨
	RelativeTransform = WorldTransform;
}

//부모의 월드와 나의 로컬을 바탕으로 월드를 계산
void FPSceneComponent::CalculateWorldTransform()
{
	//자신이 Root Component인 경우
	if (ParentComponent == nullptr) { WorldTransform = RelativeTransform; return; };

	//Scale 계산 (부모 월드 스케일 * 로컬 스케일)
	WorldTransform.Scale = ParentComponent->WorldTransform.Scale * RelativeTransform.Scale;

	//Rotation 계산 (부모 사원수 회전 * 로컬 사원수 회전)
	WorldTransform.QuaternionRotation = (ParentComponent->WorldTransform.QuaternionRotation * RelativeTransform.QuaternionRotation).Normalize();
	WorldTransform.Rotation = WorldTransform.QuaternionRotation.ToEuler();

	//Location 계산 (부모 위치 + Rotate(부모 회전 사원수, (자식 위치 * 부모 크기)) 
	WorldTransform.Location = ParentComponent->WorldTransform.Location + 
		Rotate(ParentComponent->WorldTransform.QuaternionRotation, (RelativeTransform.Location * ParentComponent->WorldTransform.Scale));

}

//부모의 월드와 나의 월드로 나의 로컬을 계산
void FPSceneComponent::CalculateLocalTransform()
{
	//자신이 Root Component인 경우
	if (ParentComponent == nullptr) { RelativeTransform = WorldTransform; return; };

	//부모의 월드 스케일 역
	FPVector3 ParentInverseScale = { 1 / ParentComponent->WorldTransform.Scale.x, 1 / ParentComponent->WorldTransform.Scale.y, 1 / ParentComponent->WorldTransform.Scale.z };

	//부모의 월드 회전 켤례 사원수
	FPQuaternion ParentConjugateRotation = Inverse(ParentComponent->WorldTransform.QuaternionRotation);

	//부모의 월드 이동 역
	FPVector3 ParentInverseLocation = Rotate(ParentConjugateRotation, -ParentComponent->WorldTransform.Location) * ParentInverseScale;

	//재계산된 로컬

	//스케일
	RelativeTransform.Scale = WorldTransform.Scale * ParentInverseScale;

	//회전
	RelativeTransform.QuaternionRotation = (ParentConjugateRotation * WorldTransform.QuaternionRotation).Normalize();
	RelativeTransform.Rotation = RelativeTransform.QuaternionRotation.ToEuler();

	//이동
	RelativeTransform.Location = ParentInverseLocation +
		Rotate(ParentConjugateRotation, (RelativeTransform.Location * ParentInverseScale));
}

void FPSceneComponent::AttachChildComponent(FPSceneComponent* Child)
{
	ChildComponent.push_back(Child);
}

void FPSceneComponent::DetachChildComponent(FPSceneComponent* Child)
{
	auto It = std::find(ChildComponent.begin(), ChildComponent.end(), Child);

	if (It != ChildComponent.end())
	{
		std::cout << "erase" << "\n";
		ChildComponent.erase(It);
	}
	else
	{
		return;
	}
}